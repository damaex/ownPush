#include <iostream>
#include "PushServer.h"
#include "Log.h"
#include "TestUserProvider.h"

int main(int argc, char *argv[]) {
    std::shared_ptr<Log> log = std::make_shared<Log>();
    std::shared_ptr<TestUserProvider> userProvider = std::make_shared<TestUserProvider>();

    try {
        asio::io_context io_context;
        std::shared_ptr<PushServer> server = std::make_shared<PushServer>(io_context, log, userProvider);
        server->start();

        io_context.run();
    } catch (std::exception &e) {
        log->exception(e);
    }

    return 0;
}
