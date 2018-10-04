#include "ConsoleLog.h"
#include "Client.h"

int main() {
    std::string host = "localhost";
    std::string userID = "004917653193752";
    std::string secret = "2gnWI4ObtpS3xWaRAjaQGlnmi504hiOpvOWEBG27Hex1L7PTIp6erKzZHtrlTPeR";
    std::shared_ptr<ConsoleLog> log = std::make_shared<ConsoleLog>();

    log->writeLine("ownPush Client");
    log->writeLine("~~~~~~~~~~~~~~");
    log->writeLine("(enter \"exit\" to cancel)");
    log->writeLine("");

    asio::io_context io_context;
    Client cl(io_context, log, host, userID, secret);
    cl.start();

    std::thread t([&io_context]() { io_context.run(); });

    while (log->readLine() != "exit") {}

    cl.stop();
    t.join();
    
    return 0;
}