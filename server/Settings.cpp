#include "Settings.h"
#include <INIReader.h>

Settings::Settings(std::shared_ptr<ILog> log) {
    INIReader reader("settings.ini");

    if (reader.ParseError() == 0) {
        this->p_restName = reader.Get("rest", "name", STANDARD_REST_NAME);
        this->p_restPassword = reader.Get("rest", "password", STANDARD_REST_PASSWORD);
        this->p_restPort = static_cast<unsigned short>(reader.GetInteger("rest", "port", STANDARD_REST_PORT));
    } else {
        log->writeLine("Couldn't parse 'settings.ini' !!!");
    }
}

std::string Settings::getRestName() {
    return this->p_restName;
}

std::string Settings::getRestPassword() {
    return this->p_restPassword;
}

unsigned short Settings::getRestPort() {
    return this->p_restPort;
}
