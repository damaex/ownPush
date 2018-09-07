#include "Client.h"

void Client::doRead() {
    auto self(shared_from_this());
    std::vector<char> data;
    this->p_socket.async_read_some(asio::buffer(data),
                                   [this, self, data](std::error_code ec, std::size_t length) {
                                       if (!ec) {
                                           std::string str(data.begin(), data.end());
                                           this->p_handler->incomingPushData(str);
                                       }
                                   });
}

Client::Client(asio::ip::tcp::socket socket, std::shared_ptr<IHandler> handler, std::shared_ptr<ILog> log)
        : p_socket(std::move(socket)), p_handler(std::move(handler)), p_log(std::move(log)) {}

void Client::start() {
    this->doRead();
}

void Client::stop() {
    this->p_socket.close();
}

void Client::doWrite(const std::string &data) {
    auto self(shared_from_this());
    asio::async_write(this->p_socket, asio::const_buffer(data.c_str(), data.size()),
                      [this, self](std::error_code ec, std::size_t /*length*/) {
                          if (!ec) {
                              //TODO what to do next, basically nothing
                          }
                      });
}
