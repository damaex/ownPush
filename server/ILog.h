#ifndef OWNPUSH_ILOG_H
#define OWNPUSH_ILOG_H

#include <string>
#include <exception>

#ifdef _WIN32
#define NEWLINE "\r\n"
#elif defined macintosh // OS 9
#define NEWLINE "\r"
#else
#define NEWLINE "\n" // Unix & Mac OS X uses \n
#endif

class ILog {
protected:
    virtual void write(const std::string &text) = 0;

public:
    void writeLine(const std::string &text) {
        this->write(text + NEWLINE);
    }

    void exception(std::exception &ex) {
        this->write("Exception: ");
        this->writeLine(ex.what());
    }
};

#endif //OWNPUSH_ILOG_H
