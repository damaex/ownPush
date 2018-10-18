#ifndef TESTCLIENT_CLIENT_H
#define TESTCLIENT_CLIENT_H

#include <asio.hpp>
#include <asio/ssl.hpp>
#include "../../server/ILog.h"
#include "../../server/ConnectionObject.h"

constexpr auto BUFFER_SIZE = 1024;
constexpr auto OWNPUSH_PORT = 7951;

class Client {
private:
	std::shared_ptr<ILog> p_log;
	std::string p_clientID;
	std::string p_secret;

	std::string p_host;

	asio::io_context& p_io_context;
	asio::ssl::stream<asio::ip::tcp::socket> p_socket;

	void doRead();
	void doWrite(const std::string &data);

	size_t stosize(const std::string &data);
	std::string hash(const std::string &data);

	void sendRequest();
	void sendData(const ConnectionObject &co);
	void dataHandler(const std::string &data);
	void pushReceived(const std::string &data);

	void doConnect();
	void closeSocket();
	void didDisconnect();
	void didDisconnect(bool directReconnect);

public:
	Client(asio::io_context &io_context, asio::ssl::context& context, std::shared_ptr<ILog> log, const std::string &host, const std::string &clientID, const std::string &secret);

	void start();
	void stop();
};

#endif //TESTCLIENT_CLIENT_H
