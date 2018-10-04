#ifndef OWNPUSH_CONNECTIONOBJECT_H
#define OWNPUSH_CONNECTIONOBJECT_H

#include <string>
#include <nlohmann/json.hpp>

class ConnectionObject {
public:

	enum Purpose
	{
		PERROR = -1,
		PREQUEST = 0,
		PCHALLENGE = 1,
		PLOGIN = 2,
		PPUSH = 3,
		PRESET = 4
	};

	ConnectionObject() = default;
	ConnectionObject(Purpose pur, const std::string &d) : purpose(pur), data(d) {}

	Purpose purpose = PERROR;
	std::string data;
};

void to_json(nlohmann::json& j, const ConnectionObject& co);

void from_json(const nlohmann::json& j, ConnectionObject& co);

#endif //OWNPUSH_CONNECTIONOBJECT_H
