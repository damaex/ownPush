#ifndef OWNPUSH_UNKNOWNCOMMANDEXCEPTION_H
#define OWNPUSH_UNKNOWNCOMMANDEXCEPTION_H

#include <exception>

class UnknownCommandException : public std::exception {
public:
	UnknownCommandException() = default;

	const char *what() const noexcept override {
		return "UnknownCommandException";
	}
};

#endif //OWNPUSH_UNKNOWNCOMMANDEXCEPTION_H
