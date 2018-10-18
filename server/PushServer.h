#ifndef OWNPUSH_SERVER_H
#define OWNPUSH_SERVER_H

#include "Client.h"
#include "RestServer.h"
#include "ConnectionObject.h"
#include "ChallengeHandler.h"

constexpr auto OWNPUSH_PORT = 7951;

class PushServer : public IHandler, public std::enable_shared_from_this<IHandler> {
private:
    asio::ip::tcp::acceptor p_acceptor;
    asio::ssl::context ssl_context;
    std::shared_ptr<ILog> p_log;
    std::vector<std::shared_ptr<Client>> p_clientList;
    std::shared_ptr<RestServer> p_restServer;
    ChallengeHandler p_challengeHandler;

    void doAccept();

    void removeClient(std::shared_ptr<IClient> cl) override;

    void handleIncoming(std::shared_ptr<IClient> cl, const ConnectionObject &co);

    void sendData(std::shared_ptr<IClient> cl, const ConnectionObject &co);

    std::string getSslPassword();

public:
    explicit PushServer(asio::io_context &io_context, std::shared_ptr<ILog> log, std::shared_ptr<IUserProvider> userProvider);

    void start();

    void stop();

    void incomingPushData(std::shared_ptr<IClient> cl, const std::string &data) override;

    std::set<std::string> getConnectedClients() override;

    bool checkConnectedClient(const std::string &id) override;

    bool pushMessage(const std::string &id, const std::string &message) override;
};

#endif //OWNPUSH_SERVER_H
