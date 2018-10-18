#ifndef OWNPUSH_CLIENT_H
#define OWNPUSH_CLIENT_H

#include <asio.hpp>
#include <asio/ssl.hpp>

#include "IHandler.h"

constexpr auto BUFFER_SIZE = 1024;

class Client : public IClient, public std::enable_shared_from_this<IClient> {
private:
	asio::ssl::stream<asio::ip::tcp::socket> ssl_socket;
	std::shared_ptr<IHandler> p_handler;

	void doRead();

public:
	explicit Client(asio::io_context& io_context, asio::ssl::context& context, std::shared_ptr<IHandler> handler, std::shared_ptr<ILog> log);

	void start();

	asio::ssl::stream<asio::ip::tcp::socket>::lowest_layer_type& socket();

	void stop() override;

	void doWrite(const std::string &data) override;
};

#endif //OWNPUSH_CLIENT_H
