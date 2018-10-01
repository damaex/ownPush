#ifndef OWNPUSH_CHALLENGEHANDLER_H
#define OWNPUSH_CHALLENGEHANDLER_H

#include "IUserProvider.h"
#include <memory>

class ChallengeHandler {
private:
	std::shared_ptr<IUserProvider> p_userProvider;

	std::string randomString(size_t length);

	std::string hash(const std::string &data);

public:
	explicit ChallengeHandler(std::shared_ptr<IUserProvider> userProvider);

	std::string createChallenge();

	bool checkLogin(const std::string &challenge, const std::string &clientID, const std::string &login);
};

#endif //OWNPUSH_CHALLENGEHANDLER_H
