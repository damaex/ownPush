#include "Client.h"

void Client::doRead() {
    auto self(shared_from_this());
    auto reply = new char[BUFFER_SIZE];

    this->ssl_socket.async_read_some(asio::buffer(reply, BUFFER_SIZE),
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

Client::Client(asio::io_context& io_context, asio::ssl::context& context, std::shared_ptr<IHandler> handler, std::shared_ptr<ILog> log)
        : IClient(std::move(log)), ssl_socket(io_context, context), p_handler(std::move(handler)) {}

void Client::start() {
    auto self(shared_from_this());

    ssl_socket.async_handshake(asio::ssl::stream_base::server, [this, self](std::error_code ec) {
        if(!ec)
            this->doRead();
        else
            this->p_handler->removeClient(self);
    });
}

asio::ssl::stream<asio::ip::tcp::socket>::lowest_layer_type& Client::socket() {
    return this->ssl_socket.lowest_layer();
}

void Client::stop() {
    std::error_code ec;
    this->ssl_socket.shutdown(ec);

    this->socket().shutdown(asio::ip::tcp::socket::shutdown_both);
    this->socket().close();
}

void Client::doWrite(const std::string &data) {
    auto self(shared_from_this());
    //add length to sender
    std::string toSend = std::to_string(data.length()) + ":" + data;

    asio::async_write(this->ssl_socket, asio::const_buffer(toSend.c_str(), toSend.size()),
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
