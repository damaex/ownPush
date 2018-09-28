#include "ChallengeHandler.h"

std::string ChallengeHandler::createChallenge() {
	return "48THzYSsnXyOfEZKz0hQeiDc2VBN41rbRNUuWGlRH1ggmyDfzIiRK5fwdatZFTrC";
}

bool ChallengeHandler::checkLogin(const std::string &challenge, const std::string &clientID, const std::string &login) {
	//get secret for client ID
	//SHA(challenge + secret) == login
}