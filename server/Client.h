#ifndef OWNPUSH_CLIENT_H
#define OWNPUSH_CLIENT_H

#include <asio.hpp>
#include "IHandler.h"

constexpr auto BUFFER_SIZE = 1024;

class Client : public IClient, public std::enable_shared_from_this<IClient> {
private:
	asio::ip::tcp::socket p_socket;
	std::shared_ptr<IHandler> p_handler;

	void doRead();

public:
	explicit Client(asio::ip::tcp::socket socket, std::shared_ptr<IHandler> handler, std::shared_ptr<ILog> log);

	void start();

	void stop() override;

	void doWrite(const std::string &data) override;
};

#endif //OWNPUSH_CLIENT_H
