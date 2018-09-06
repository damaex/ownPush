#ifndef OWNPUSH_SERVER_H
#define OWNPUSH_SERVER_H

#include "Client.h"
#include "RestServer.h"

#define OWNPUSH_PORT 7951

class PushServer : public Handler, public std::enable_shared_from_this<Handler> {
private:
    asio::ip::tcp::acceptor p_acceptor;
    std::shared_ptr<ILog> p_log;
    std::vector<std::shared_ptr<Client>> p_clientList;
    std::shared_ptr<RestServer> p_restServer;

    void doAccept();

public:
    explicit PushServer(asio::io_context &io_context, std::shared_ptr<ILog> log);

    void start();

    void stop();

    void incomingPushData(const std::string &data) override;

    std::set<std::string> getConnectedClients() override;

    bool checkConnectedClient(const std::string &id) override;

    bool pushMessage(const std::string &id, const std::string &message) override;
};

#endif //OWNPUSH_SERVER_H
