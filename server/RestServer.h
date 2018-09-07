#ifndef OWNPUSH_RESTSERVER_H
#define OWNPUSH_RESTSERVER_H

#include "Settings.h"
#include "IHandler.h"
#include <server_http.hpp>

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

class RestServer {
private:
    Settings p_settings;
    std::shared_ptr<IHandler> p_handler;
    std::shared_ptr<ILog> p_log;
    HttpServer p_httpServer;
    std::thread p_serverThread;

    bool p_isRunning = false;

    void init();

    bool checkAdmin(SimpleWeb::CaseInsensitiveMultimap &map);

    void sendError(std::shared_ptr<HttpServer::Response> response);

    std::string createAnswer(bool isOK);

public:
    explicit RestServer(std::shared_ptr<IHandler> handler, std::shared_ptr<ILog> log);

    void start();

    void stop();
};

#endif //OWNPUSH_RESTSERVER_H
