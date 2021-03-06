#include <utility>

#include "RestServer.h"
#include <nlohmann/json.hpp>

RestServer::RestServer(std::shared_ptr<IHandler> handler, std::shared_ptr<ILog> log, Settings& settings)
        : p_handler(std::move(handler)),
          p_log(std::move(log)),
          p_settings(settings),
          p_httpServer(p_settings.getSslCertificateFile(), p_settings.getSslPrivateKeyFile())
{
    this->p_httpServer.config.port = this->p_settings.getRestPort();
    this->init();
}

void RestServer::start() {
    if (!this->p_isRunning) {
        this->p_isRunning = true;
        this->p_serverThread = std::thread([this]() {
            // Start server
            this->p_httpServer.start();
        });
    }
}

void RestServer::stop() {
    if (this->p_isRunning) {
        this->p_serverThread.join();
        this->p_isRunning = false;
    }
}

void RestServer::init() {
    this->p_httpServer.on_error = [](std::shared_ptr<HttpsServer::Request> /*request*/,
                                     const SimpleWeb::error_code & /*ec*/) {
        // Handle errors here
        // Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled
    };

    this->p_httpServer.default_resource["GET"] = [this](std::shared_ptr<HttpsServer::Response> response,
                                                        std::shared_ptr<HttpsServer::Request> request) {
        this->sendError(response);
    };

    this->p_httpServer.default_resource["POST"] = [this](std::shared_ptr<HttpsServer::Response> response,
                                                         std::shared_ptr<HttpsServer::Request> request) {
        this->sendError(response);
    };

    this->p_httpServer.resource["^/clients"]["GET"] = [this](std::shared_ptr<HttpsServer::Response> response,
                                                             std::shared_ptr<HttpsServer::Request> request) {
        if (this->checkAdmin(request->header)) {
            nlohmann::json jsonData(this->p_handler->getConnectedClients());
            response->write(jsonData.dump(4));
        } else {
            this->sendError(response);
        }
    };

    this->p_httpServer.resource["^/online"]["GET"] = [this](std::shared_ptr<HttpsServer::Response> response,
                                                            std::shared_ptr<HttpsServer::Request> request) {
        auto queryFields = request->header;
        auto itID = queryFields.find("id");

        if (this->checkAdmin(request->header) && itID != queryFields.end()) {
            response->write(this->createAnswer(this->p_handler->checkConnectedClient(itID->second)));
        } else {
            this->sendError(response);
        }
    };

    this->p_httpServer.resource["^/push"]["POST"] = [this](std::shared_ptr<HttpsServer::Response> response,
                                                           std::shared_ptr<HttpsServer::Request> request) {
        auto queryFields = request->header;
        auto itID = queryFields.find("id");
        auto body = request->content.string();

        if (this->checkAdmin(request->header) && itID != queryFields.end() && !body.empty()) {
            response->write(this->createAnswer(this->p_handler->pushMessage(itID->second, body)));
        } else {
            this->sendError(response);
        }
    };
}

bool RestServer::checkAdmin(SimpleWeb::CaseInsensitiveMultimap &map) {
    auto itName = map.find("name");
    auto itPassword = map.find("password");

    return itName != map.end()
           && itPassword != map.end()
           && this->p_settings.getRestName() == itName->second
           && this->p_settings.getRestPassword() == itPassword->second;
}

void RestServer::sendError(std::shared_ptr<HttpsServer::Response> response) {
    response->write(SimpleWeb::StatusCode::client_error_not_found, "not the droid you are looking for");
}

std::string RestServer::createAnswer(bool isOK) {
    nlohmann::json jsonData = {
            {"result", isOK}
    };

    std::string contentType = "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-Type: text/event-stream\r\nCache-Control: no-cache'\r\n\r\n";
    return jsonData.dump(4);
}
