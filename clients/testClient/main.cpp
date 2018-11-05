#if defined(_MSC_VER) && defined(VLD_ENABLED) && defined(_DEBUG)
#include <vld.h>
#endif

#include <thread>
#include "Client.h"
#include "ConsoleLog.h"

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
    asio::ssl::context ctx(asio::ssl::context::tlsv12);
	try {
		ctx.load_verify_file(log->getExecutablePath() + log->getPathDelimeter() + "ca.pem");
	} catch (std::exception &e) {
		log->exception(e);
		log->writeLine("Could not load \"ca.pem\" for certificate verification");
		return 1;
	}

    Client cl(io_context, ctx, log, host, userID, secret);
    cl.start();

    std::thread t([&io_context]() { io_context.run(); });

    while (log->readLine() != "exit") {}

    cl.stop();
    t.join();
    
    return 0;
}