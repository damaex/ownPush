#include "PushServer.h"
#include "exceptions/ChallengeIncorrectException.h"
#include "exceptions/ClientIDAlreadySetException.h"
#include "exceptions/UnknownCommandException.h"
#include <algorithm>

PushServer::PushServer(asio::io_context &io_context, std::shared_ptr<ILog> log, std::shared_ptr<IUserProvider> userProvider)
        : p_acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v6(), OWNPUSH_PORT)),
          p_challengeHandler(std::move(userProvider)),
          p_log(std::move(log)) {}

void PushServer::incomingPushData(std::shared_ptr<IClient> cl, const std::string &data) {
    try {
        json incData = json::parse(data);
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
    this->p_acceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket) {
        if (!ec) {
            std::shared_ptr<Client> cl = std::make_shared<Client>(std::move(socket), shared_from_this(), this->p_log);
            this->p_clientList.push_back(cl);
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
            json toSend = answer;

            this->p_log->writeLine("OUT: " + toSend.dump());

            //send answer
            cl->doWrite(toSend.dump());

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

std::set<std::string> PushServer::getConnectedClients() {
    std::set<std::string> data = {"test1", "test2"};
    return data;
}

void PushServer::start() {
    this->p_restServer = std::make_shared<RestServer>(shared_from_this(), this->p_log);
    this->p_restServer->start();

    this->doAccept();
}

void PushServer::stop() {
    for(auto client : this->p_clientList) {
        client->stop();
    }
}

bool PushServer::checkConnectedClient(const std::string &id) {
    return false;
}

bool PushServer::pushMessage(const std::string &id, const std::string &message) {
    return false;
}
