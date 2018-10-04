#include "ConnectionObject.h"

void to_json(nlohmann::json& j, const ConnectionObject& co) {
	j = nlohmann::json{ {"purpose", co.purpose}, {"data", co.data} };
}

void from_json(const nlohmann::json& j, ConnectionObject& co) {
	co.purpose = static_cast<ConnectionObject::Purpose>(j.at("purpose").get<int>());
	co.data = j.at("data").get<std::string>();
}

