#ifndef OWNPUSH_SETTINGS_H
#define OWNPUSH_SETTINGS_H

#include "ILog.h"
#include <memory>

#define STANDARD_REST_NAME "root"
#define STANDARD_REST_PASSWORD "toor"
#define STANDARD_REST_PORT 7952
#define STANDARD_SSL_CERTIFICATE_FILE "ownpush.crt"
#define STANDARD_SSL_PRIVATE_KEY_FILE "ownpush.key"

class Settings {
private:
    std::string p_restName = STANDARD_REST_NAME;
    std::string p_restPassword = STANDARD_REST_PASSWORD;
    std::string p_sslCertificateFile = STANDARD_SSL_CERTIFICATE_FILE;
    std::string p_sslPrivateKeyFile = STANDARD_SSL_PRIVATE_KEY_FILE;
    unsigned short p_restPort = STANDARD_REST_PORT;

    std::string p_path;

public:
    explicit Settings(std::shared_ptr<ILog> log);

    std::string getRestName();

    std::string getRestPassword();

    unsigned short getRestPort();

    std::string getSslCertificateFile();

    std::string getSslPrivateKeyFile();
};

#endif //OWNPUSH_SETTINGS_H
