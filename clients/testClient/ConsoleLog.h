#ifndef TESTCLIENT_CONSOLELOG_H
#define TESTCLIENT_CONSOLELOG_H

#include <iostream>
#include "../../server/ILog.h"

class ConsoleLog : public ILog {
private:
	void write(const std::string &text) override {
		std::cout << text;
	}

public:
	std::string readLine() {
		std::string in;
		std::cin >> in;

		return in;
	}
};

#endif //TESTCLIENT_CONSOLELOG_H