#ifndef OWNPUSH_IHANDLER_H
#define OWNPUSH_IHANDLER_H

#include "IClient.h"
#include <memory>
#include <set>

class IHandler {
public:
    virtual void incomingPushData(std::shared_ptr<IClient> cl, const std::string &data) = 0;

    virtual void removeClient(std::shared_ptr<IClient> cl) = 0;

    virtual std::set<std::string> getConnectedClients() = 0;

    virtual bool checkConnectedClient(const std::string &id) = 0;

    virtual bool pushMessage(const std::string &id, const std::string &message) = 0;
};

#endif //OWNPUSH_IHANDLER_H
