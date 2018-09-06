#include <iostream>
#include "PushServer.h"
#include "Log.h"

int main(int argc, char *argv[]) {
    std::shared_ptr<Log> log = std::make_shared<Log>();

    try {
        asio::io_context io_context;
        std::shared_ptr<PushServer> server = std::make_shared<PushServer>(io_context, log);
        server->start();

        io_context.run();
    } catch (std::exception &e) {
        log->exception(e);
    }

    return 0;
}
