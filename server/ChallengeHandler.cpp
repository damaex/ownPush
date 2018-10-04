#include "ChallengeHandler.h"
#include <random>
#include <algorithm>
#include <vector>
#include <sha2_512.h>

std::string ChallengeHandler::randomString(size_t length) {
    const auto ch_set = std::vector<char>(
        { '0','1','2','3','4',
        '5','6','7','8','9',
        'A','B','C','D','E','F',
        'G','H','I','J','K',
        'L','M','N','O','P',
        'Q','R','S','T','U',
        'V','W','X','Y','Z',
        'a','b','c','d','e','f',
        'g','h','i','j','k',
        'l','m','n','o','p',
        'q','r','s','t','u',
        'v','w','x','y','z'
        });

    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, static_cast<int>(ch_set.size() - 1));
    auto randchar = [ch_set, &dist, &rng]() {return ch_set[dist(rng)]; };

    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

std::string ChallengeHandler::hash(const std::string &data) {
    return Chocobo1::SHA2_512().addData(data.c_str(), static_cast<long>(data.length())).finalize().toString();
}

ChallengeHandler::ChallengeHandler(std::shared_ptr<IUserProvider> userProvider)
    : p_userProvider(userProvider) {}

std::string ChallengeHandler::createChallenge() {
    return this->randomString(64);
}

bool ChallengeHandler::checkLogin(const std::string &challenge, const std::string &clientID, const std::string &login) {
    //get secret for client ID
    std::string secret = p_userProvider->getClientSecret(clientID);
    //check hash against login data
    return this->hash(challenge + secret) == login;
}