#include "TestUserProvider.h"

TestUserProvider::TestUserProvider() {
	this->p_users.insert(std::pair<std::string, std::string>("004917653193751", "YOgCN3fU77FykBmWEnRIOBXmN9hEoGD4GRcpA7pX8Mo3TgxCYeddkLPkrJ8v8mz6"));
}

std::string TestUserProvider::getClientSecret(const std::string &clientID) {
	auto it = this->p_users.find(clientID);
	if (it != this->p_users.end())
		return it->second;

	return "";
}