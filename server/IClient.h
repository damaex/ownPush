#ifndef OWNPUSH_ICLIENT_H
#define OWNPUSH_ICLIENT_H

#include <string>

class IClient {
private:
	std::string clientID;
public:

	void setClientID(std::string value) {
		this->clientID = value;
	}

	std::string getClientID() {
		return this->clientID;
	}

	bool hasClientID() {
		return !this->clientID.empty();
	}

	virtual void stop() = 0;

	virtual void doWrite(const std::string &data) = 0;
};

#endif //OWNPUSH_ICLIENT_H
