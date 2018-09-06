#ifndef OWNPUSH_LOG_H
#define OWNPUSH_LOG_H

#include <iostream>
#include "ILog.h"

class Log : public ILog {
private:
    void write(const std::string &text) override {
        std::cout << text;
    }
};

#endif //OWNPUSH_LOG_H
