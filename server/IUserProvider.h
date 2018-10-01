#ifndef OWNPUSH_IUSERPROVIDER_H
#define OWNPUSH_IUSERPROVIDER_H

#include <string>

class IUserProvider {
public:
    virtual std::string getClientSecret(const std::string &clientID) = 0;
};

#endif //OWNPUSH_IUSERPROVIDER_H
