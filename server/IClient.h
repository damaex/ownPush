#ifndef OWNPUSH_ICLIENT_H
#define OWNPUSH_ICLIENT_H

#include <string>
#include "ILog.h"

class IClient {
private:
	std::shared_ptr<ILog> p_log;

	std::string p_clientID;
	std::string p_challenge;

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

	virtual void stop() = 0;

	virtual void doWrite(const std::string &data) = 0;

	std::shared_ptr<ILog> getLog() {
		return this->p_log;
	}
};

#endif //OWNPUSH_ICLIENT_H
