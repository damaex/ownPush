#ifndef OWNPUSH_HANDLER_H
#define OWNPUSH_HANDLER_H

#include <string>
#include <set>

class Handler {
public:
    virtual void incomingPushData(const std::string &data) = 0;

    virtual std::set<std::string> getConnectedClients() = 0;

    virtual bool checkConnectedClient(const std::string &id) = 0;

    virtual bool pushMessage(const std::string &id, const std::string &message) = 0;
};

#endif //OWNPUSH_HANDLER_H
