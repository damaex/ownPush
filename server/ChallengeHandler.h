#ifndef OWNPUSH_CHALLENGEHANDLER_H
#define OWNPUSH_CHALLENGEHANDLER_H

#include <string>

class ChallengeHandler {
public:
	std::string createChallenge();

	bool checkLogin(const std::string &challenge, const std::string &clientID, const std::string &login);
};

#endif //OWNPUSH_CHALLENGEHANDLER_H
