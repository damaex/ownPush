#include "ConnectionObject.h"

void to_json(json& j, const ConnectionObject& co) {
	j = json{ {"purpose", co.purpose}, {"data", co.data} };
}

void from_json(const json& j, ConnectionObject& co) {
	co.purpose = static_cast<ConnectionObject::Purpose>(j.at("purpose").get<int>());
	co.data = j.at("data").get<std::string>();
}

