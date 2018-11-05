#include "PushServer.h"
#include "exceptions/ChallengeIncorrectException.h"
#include "exceptions/ClientIDAlreadySetException.h"
#include "exceptions/UnknownCommandException.h"
#include <algorithm>
#include <iterator>

PushServer::PushServer(asio::io_context &io_context, std::shared_ptr<ILog> log, std::shared_ptr<IUserProvider> userProvider)
        : p_acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v6(), OWNPUSH_PORT)),
          ssl_context(asio::ssl::context::tlsv12),
          p_challengeHandler(std::move(userProvider)),
          p_log(log),
          p_settings(log)
{
    ssl_context.use_certificate_chain_file(p_settings.getSslCertificateFile());
    ssl_context.use_private_key_file(p_settings.getSslPrivateKeyFile(), asio::ssl::context::pem);
}

void PushServer::incomingPushData(std::shared_ptr<IClient> cl, const std::string &data) {
    try {
        nlohmann::json incData = nlohmann::json::parse(data);
        ConnectionObject co = incData;

        this->p_log->writeLine("INC: " + incData.dump());
        this->handleIncoming(cl, co);
    }
    catch (std::exception &e) {
        //incorrect data -> delete client and connection
        this->p_log->exception(e);
        this->removeClient(cl);
    }
}

void PushServer::doAccept() {
    std::shared_ptr<Client> cl = std::make_shared<Client>(this->p_acceptor.get_io_context(), this->ssl_context, shared_from_this(), this->p_log);

    this->p_acceptor.async_accept(cl->socket(), [this, cl](std::error_code ec) {
        if (!ec) {
            this->p_clientList.push_back(cl);
            this->p_log->writeLine("Client connected");
            cl->start();
        }

        this->doAccept();
    });
}

void PushServer::removeClient(std::shared_ptr<IClient> cl) {
    cl->stop();
    this->p_clientList.erase(std::remove(this->p_clientList.begin(), this->p_clientList.end(), cl), this->p_clientList.end());
}

void PushServer::handleIncoming(std::shared_ptr<IClient> cl, const ConnectionObject &co) {
    switch (co.purpose) {
        case ConnectionObject::Purpose::PREQUEST: {
            if (cl->hasClientID()) {
                throw ClientIDAlreadySetException();
            }

            cl->setClientID(co.data);
            
            //generate challenge
            std::string challenge = this->p_challengeHandler.createChallenge();

            //set challenge to client
            cl->setChallenge(challenge);

            //generate answer
            ConnectionObject answer(ConnectionObject::Purpose::PCHALLENGE, challenge);
            this->sendData(cl, answer);

            break;
        }
        case ConnectionObject::Purpose::PLOGIN: {
            if (!cl->hasClientID()) {
                throw UnknownCommandException();
            }

            //check the login
            if (this->p_challengeHandler.checkLogin(cl->getChallenge(), cl->getClientID(), co.data)) {
                cl->login();
            } else {
                throw ChallengeIncorrectException();
            }

            break;
        }
        case ConnectionObject::Purpose::PERROR: {
            break;
        }
        default: {
            throw UnknownCommandException();
        }
    }
}

void PushServer::sendData(std::shared_ptr<IClient> cl, const ConnectionObject &co) {
    nlohmann::json toSend = co;
    this->p_log->writeLine("OUT: " + toSend.dump());

    //send answer
    cl->doWrite(toSend.dump());
}

std::set<std::string> PushServer::getConnectedClients() {
    std::set<std::string> data;

    //iterate over client list
    for (const std::shared_ptr<Client> &client : this->p_clientList) {
        //fill set with logged in clients
        if (client->isLoggedIn())
            data.emplace(client->getClientID());
    }

    return data;
}

void PushServer::start() {
    this->p_restServer = std::make_shared<RestServer>(shared_from_this(), this->p_log, this->p_settings);
    this->p_restServer->start();

    this->doAccept();
}

void PushServer::stop() {
    for(const std::shared_ptr<Client> &client : this->p_clientList)
        client->stop();
}

bool PushServer::checkConnectedClient(const std::string &id) {
    //check if clientID is connected

    auto result = std::find_if(this->p_clientList.begin(), this->p_clientList.end(),
        [&](const std::shared_ptr<Client> &client) { return client->getClientID() == id; }
    );

    return result != this->p_clientList.end();
}

bool PushServer::pushMessage(const std::string &id, const std::string &message) {
    //find client by id
    auto result = std::find_if(this->p_clientList.begin(), this->p_clientList.end(),
        [&](const std::shared_ptr<Client> &client) { return client->getClientID() == id; }
    );

    if (result == this->p_clientList.end())
        return false;

    //send message
    ConnectionObject answer(ConnectionObject::Purpose::PPUSH, message);
    this->sendData(*result, answer);

    return true;
}
