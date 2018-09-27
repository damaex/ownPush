#ifndef OWNPUSH_CLIENTIDALREADYSETEXCEPTION_H
#define OWNPUSH_CLIENTIDALREADYSETEXCEPTION_H

#include <exception>

class ClientIDAlreadySetException : public std::exception {
public:
	ClientIDAlreadySetException() = default;

	const char *what() const noexcept override {
		return "ClientIDAlreadySetException";
	}
};

#endif //OWNPUSH_CLIENTIDALREADYSETEXCEPTION_H
