#include "Client.h"

void Client::doRead() {
    auto self(shared_from_this());
    auto reply = new char[BUFFER_SIZE];

    this->p_socket.async_read_some(asio::buffer(reply, BUFFER_SIZE),
                                   [this, self, reply](std::error_code ec, std::size_t length) {
                                       if (!ec) {
                                            std::string str(reply, length);
                                            size_t data_length = self->stosize(str.substr(0, str.find(':')));
                                            std::string data = str.substr(str.find(':') + 1);

                                            if(data.length() == data_length)
                                                this->p_handler->incomingPushData(self, data);

                                            this->doRead();
                                       } else {
                                           if (asio::error::eof == ec ||
                                               asio::error::connection_reset == ec) {
                                               self->getLog()->writeLine("Client disconnected");
                                               this->p_handler->removeClient(self);
                                           }
                                       }

                                       delete[] reply;
                                   });
}

Client::Client(asio::ip::tcp::socket socket, std::shared_ptr<IHandler> handler, std::shared_ptr<ILog> log)
        : IClient(std::move(log)), p_socket(std::move(socket)), p_handler(std::move(handler)) {}

void Client::start() {
    this->doRead();
}

void Client::stop() {
    this->p_socket.shutdown(asio::ip::tcp::socket::shutdown_both);
    this->p_socket.close();
}

void Client::doWrite(const std::string &data) {
    auto self(shared_from_this());
    //add length to sender
    std::string toSend = std::to_string(data.length()) + ":" + data;

    asio::async_write(this->p_socket, asio::const_buffer(toSend.c_str(), toSend.size()),
                      [this, self](std::error_code ec, std::size_t /*length*/) {
                          if (ec) {
                              if (asio::error::eof == ec ||
                                  asio::error::connection_reset == ec) {
                                  self->getLog()->writeLine("Client disconnected");
                                  this->p_handler->removeClient(self);
                              }
                              else {
                                  self->getLog()->writeLine(ec.message());
                              }
                          }
                      });
}
