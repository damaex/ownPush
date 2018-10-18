#ifndef OWNPUSH_ILOG_H
#define OWNPUSH_ILOG_H

#include <string>
#include <exception>

#ifdef _WIN32
#include <Windows.h>
#else
#include <climits>
#include <cstring>
#include <unistd.h>
#endif

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

    std::string getExecutablePath() {
#ifdef _WIN32
        HMODULE hModule = GetModuleHandleW(NULL);
        WCHAR path[MAX_PATH];
        GetModuleFileNameW(hModule, path, MAX_PATH);

        std::wstring ws(path);
        std::string full(ws.begin(), ws.end());

        return full.substr(0, full.find_last_of("\\/"));
#else
        char dest[PATH_MAX];
        memset(dest, 0, sizeof(dest)); // readlink does not null terminate!
        if (readlink("/proc/self/exe", dest, PATH_MAX) == -1) {
            this->writeLine("could not get own path");
            return "";
        }

        std::string exePath = std::string(dest);
        return exePath.substr(0, exePath.find_last_of('/')); //remove executable
#endif
    }

    std::string getPathDelimeter() {
#ifdef _WIN32
        return "\\";
#else
        return "/";
#endif
    }
};

#endif //OWNPUSH_ILOG_H
