#ifndef OWNPUSH_CLIENT_H
#define OWNPUSH_CLIENT_H

#include <asio.hpp>
#include "Handler.h"
#include "ILog.h"

class Client : public std::enable_shared_from_this<Client> {
private:
    asio::ip::tcp::socket p_socket;
    std::shared_ptr<Handler> p_handler;
    std::shared_ptr<ILog> p_log;

    void doRead();

public:
    explicit Client(asio::ip::tcp::socket socket, std::shared_ptr<Handler> handler, std::shared_ptr<ILog> log);

    void start();

    void stop();

    void doWrite(const std::string &data);
};

#endif //OWNPUSH_CLIENT_H
