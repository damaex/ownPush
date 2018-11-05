#ifndef OWNPUSH_RESTSERVER_H
#define OWNPUSH_RESTSERVER_H

#include <server_https.hpp>
#include "Settings.h"
#include "IHandler.h"

using HttpsServer = SimpleWeb::Server<SimpleWeb::HTTPS>;

class RestServer {
private:
	Settings& p_settings;
    std::shared_ptr<IHandler> p_handler;
    std::shared_ptr<ILog> p_log;
    HttpsServer p_httpServer;
    std::thread p_serverThread;

    bool p_isRunning = false;

    void init();

    bool checkAdmin(SimpleWeb::CaseInsensitiveMultimap &map);

    void sendError(std::shared_ptr<HttpsServer::Response> response);

    std::string createAnswer(bool isOK);

public:
    RestServer(std::shared_ptr<IHandler> handler, std::shared_ptr<ILog> log, Settings& settings);

    void start();

    void stop();
};

#endif //OWNPUSH_RESTSERVER_H
