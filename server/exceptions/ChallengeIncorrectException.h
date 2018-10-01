#ifndef OWNPUSH_CHALLENGEINCORRECTEXCEPTION_H
#define OWNPUSH_CHALLENGEINCORRECTEXCEPTION_H

#include <exception>

class ChallengeIncorrectException : public std::exception {
public:
	ChallengeIncorrectException() = default;

	const char *what() const noexcept override {
		return "ChallengeIncorrectException";
	}
};

#endif //OWNPUSH_CHALLENGEINCORRECTEXCEPTION_H
