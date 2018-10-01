#ifndef OWNPUSH_TESTUSERPROVIDER_H
#define OWNPUSH_TESTUSERPROVIDER_H

#include "IUserProvider.h"
#include <map>

class TestUserProvider : public IUserProvider {
private:
	std::map<std::string, std::string> p_users;

public:
	TestUserProvider();

	std::string getClientSecret(const std::string &clientID);
};

#endif //OWNPUSH_TESTUSERPROVIDER_H
