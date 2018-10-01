#ifndef OWNPUSH_ICLIENT_H
#define OWNPUSH_ICLIENT_H

#include <string>
#include <sstream>
#include "ILog.h"

class IClient {
private:
	std::shared_ptr<ILog> p_log;

	std::string p_clientID;
	std::string p_challenge;
	bool p_loggedIn = false;

public:

	IClient(std::shared_ptr<ILog> log) : p_log(log) {}

	void setClientID(const std::string &value) {
		this->p_clientID = value;
	}

	std::string getClientID() {
		return this->p_clientID;
	}

	bool hasClientID() {
		return !this->p_clientID.empty();
	}

	void setChallenge(const std::string &value) {
		this->p_challenge = value;
	}

	std::string getChallenge() {
		return this->p_challenge;
	}

	void login() {
		p_loggedIn = true;

		this->p_log->writeLine("User '" + this->p_clientID + "' logged in");
	}

	bool isLoggedIn() {
		return this->p_loggedIn;
	}

	virtual void stop() = 0;

	virtual void doWrite(const std::string &data) = 0;

	std::shared_ptr<ILog> getLog() {
		return this->p_log;
	}

	size_t stosize(const std::string &data) {
		std::stringstream sstream(data);
		size_t result;
		sstream >> result;

		return result;
	}
};

#endif //OWNPUSH_ICLIENT_H
