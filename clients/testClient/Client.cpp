#include "Client.h"
#include "../../server/exceptions/UnknownCommandException.h"
#include <sha2_512.h>

void Client::doRead() {
	auto reply = new char[BUFFER_SIZE];

	this->p_socket.async_read_some(asio::buffer(reply, BUFFER_SIZE),
		[this, reply](std::error_code ec, std::size_t length) {
		if (!ec) {
			std::string str(reply, length);
			size_t data_length = this->stosize(str.substr(0, str.find(':')));
			std::string data = str.substr(str.find(':') + 1);

			if (data.length() == data_length)
				this->dataHandler(data);

			this->doRead();
		}
		else {
			if (asio::error::eof == ec ||
				asio::error::connection_reset == ec) {
				this->p_log->writeLine("Server disconnected");
				this->didDisconnect();
			}
		}

		delete[] reply;
	});
}

void Client::doWrite(const std::string &data) {
	//add length to sender
	std::string toSend = std::to_string(data.length()) + ":" + data;

	asio::async_write(this->p_socket, asio::const_buffer(toSend.c_str(), toSend.size()),
		[this](std::error_code ec, std::size_t /*length*/) {
		if (ec) {
			if (asio::error::eof == ec ||
				asio::error::connection_reset == ec) {
				this->p_log->writeLine("Server disconnected");
				this->didDisconnect();
			}
			else {
				this->p_log->writeLine(ec.message());
			}
		}
	});
}

size_t Client::stosize(const std::string &data) {
	std::stringstream sstream(data);
	size_t result;
	sstream >> result;

	return result;
}

std::string Client::hash(const std::string &data) {
	return Chocobo1::SHA2_512().addData(data.c_str(), static_cast<long>(data.length())).finalize().toString();
}

void Client::sendRequest() {
	ConnectionObject co(ConnectionObject::Purpose::PREQUEST, this->p_clientID);
	this->sendData(co);
}

void Client::sendData(const ConnectionObject &co) {
	nlohmann::json toSend = co;
	this->p_log->writeLine("OUT: " + toSend.dump());

	//send answer
	this->doWrite(toSend.dump());
}

void Client::dataHandler(const std::string &data) {
	try {
		nlohmann::json incData = nlohmann::json::parse(data);
		ConnectionObject co = incData;

		this->p_log->writeLine("INC: " + incData.dump());

		switch (co.purpose) {
			case ConnectionObject::Purpose::PCHALLENGE: {
				std::string hash = this->hash(co.data + this->p_secret);
				ConnectionObject answer(ConnectionObject::Purpose::PLOGIN, hash);
				this->sendData(answer);
				break;
			}
			case ConnectionObject::Purpose::PPUSH: {
				//PUSH received, handle and show
				this->pushReceived(co.data);
				break;
			}
			case ConnectionObject::Purpose::PRESET: {
				//RESET received -> reconnect
				this->didDisconnect(true);
				break;
			}
			default: {
				throw UnknownCommandException();
			}
		}

	} catch (std::exception &ex) {
		this->p_log->exception(ex);
		this->didDisconnect();
	}
}

void Client::pushReceived(const std::string &data) {
	this->p_log->writeLine("Push reiceived: " + data);
}

void Client::doConnect() {
	asio::ip::tcp::resolver resolver(this->p_io_context);
	auto endpoints = resolver.resolve(this->p_host, std::to_string(OWNPUSH_PORT));

	asio::async_connect(this->p_socket, endpoints,
		[this](std::error_code ec, asio::ip::tcp::endpoint) {
			if (!ec) {
				this->sendRequest();
				this->doRead();
			} else {
				this->p_log->writeLine("could not connect");
				this->didDisconnect();
			}
		}
	);
}

void Client::closeSocket() {
	try {
		this->p_socket.shutdown(asio::ip::tcp::socket::shutdown_both);
		this->p_socket.close();
	} catch (std::exception &ex) {
		this->p_log->exception(ex);
	}
}

void Client::didDisconnect() {
	this->didDisconnect(false);
}

void Client::didDisconnect(bool directReconnect) {
	this->closeSocket();

	if (directReconnect) {
		this->doConnect();
	} else {
		//TODO new connect after time
	}
}

Client::Client(asio::io_context &io_context, std::shared_ptr<ILog> log, const std::string &host, const std::string &clientID, const std::string &secret)
	: p_io_context(io_context), p_socket(io_context), p_log(log), p_host(host), p_clientID(clientID), p_secret(secret) {}

void Client::start() {
	this->doConnect();
}

void Client::stop() {
	this->closeSocket();
}
