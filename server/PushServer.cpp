#include "PushServer.h"

PushServer::PushServer(asio::io_context &io_context, std::shared_ptr<ILog> log)
        : p_acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), OWNPUSH_PORT)), p_log(log) {}

void PushServer::incomingPushData(const std::string &data) {
    this->p_log->writeLine("INC: " + data);
}

void PushServer::doAccept() {
    this->p_acceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket) {
        if (!ec) {
            std::shared_ptr<Client> cl = std::make_shared<Client>(std::move(socket), shared_from_this(), this->p_log);
            this->p_clientList.push_back(cl);
            cl->start();
        }

        this->doAccept();
    });
}

std::set<std::string> PushServer::getConnectedClients() {
    std::set<std::string> data = {"test1", "test2"};
    return data;
}

void PushServer::start() {
    this->p_restServer = std::make_shared<RestServer>(shared_from_this(), this->p_log);
    this->p_restServer->start();

    this->doAccept();
}

void PushServer::stop() {
    for(auto client : this->p_clientList) {
        client->stop();
    }
}

bool PushServer::checkConnectedClient(const std::string &id) {
    return false;
}

bool PushServer::pushMessage(const std::string &id, const std::string &message) {
    return false;
}
