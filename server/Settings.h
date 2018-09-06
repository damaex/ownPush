#ifndef OWNPUSH_SETTINGS_H
#define OWNPUSH_SETTINGS_H

#include "ILog.h"
#include <memory>

#define STANDARD_REST_NAME "root"
#define STANDARD_REST_PASSWORD "root"
#define STANDARD_REST_PORT 7952

class Settings {
private:
    std::string p_restName = STANDARD_REST_NAME;
    std::string p_restPassword = STANDARD_REST_PASSWORD;
    unsigned short p_restPort = STANDARD_REST_PORT;

public:
    explicit Settings(std::shared_ptr<ILog> log);

    std::string getRestName();

    std::string getRestPassword();

    unsigned short getRestPort();
};

#endif //OWNPUSH_SETTINGS_H
