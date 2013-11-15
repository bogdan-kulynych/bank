/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Authentication
 */

// #include "db.h"
#include "auth.h"
#include "config.h"

#include "utils/base64.h"
using namespace utils;

#include <iostream>

#include <string>
#include <sstream>
#include <stdexcept>
#include <regex>
#include <cassert>
#include <ctime>

#include <openssl/hmac.h>


// Wrapper for OpenSSL HMAC
std::string
hmac(const std::string& card_id, time_t timestamp)
{
    // Concatenating card_id and timestamp
    std::stringstream ss;
    ss << card_id << timestamp;

    // Converting sstream to unsigned char*
    const std::string& t = ss.str();
    size_t data_length = t.length();
    unsigned char* data = (unsigned char*)t.c_str();

    // Compute MAC binary array
    unsigned char* digest;
    digest = HMAC(EVP_sha1(),
        (unsigned char*)(config::auth::SECRET.c_str()),
        config::auth::SECRET.length(),
        data, data_length, NULL, NULL);

    // 20 is the length of an array produced by HMAC-SHA1
    size_t size = 20;

    // Convert to base64
    return base64::encode(digest, size);
}


struct Token
{
    Token(const std::string& card_id,
          time_t timestamp,
          const std::string& mac);
    explicit Token(const std::string& stringified);
    ~Token() {};

    const std::string card_id;
    const std::time_t timestamp;
    const std::string mac;
};


Token::Token(const std::string& _card_id,
             time_t _timestamp,
             const std::string& _mac):
    card_id(_card_id),
    timestamp(_timestamp),
    mac(_mac)
{
    return;
};


Token parse(const std::string& stringified)
{
    assert (stringified.length() <= 100);

    std::istringstream ss(stringified);
    char delimiter = config::auth::TOKEN_DELIMITER;

    std::string card_id;
    std::getline(ss, card_id, delimiter);

    std::string raw_timestamp;
    std::getline(ss, raw_timestamp, delimiter);
    std::stringstream ss1; ss1 << raw_timestamp;
    time_t timestamp;
    ss1 >> timestamp;

    std::string mac;
    std::getline(ss, mac, delimiter);

    return Token(card_id, timestamp, mac);
};


std::string stringify(const Token& token)
{
    std::stringstream ss;
    ss << token.card_id << config::auth::TOKEN_DELIMITER \
       << token.timestamp << config::auth::TOKEN_DELIMITER \
       << token.mac;
    return ss.str();
};


bool auth::verify_token(const std::string& stringified)
{
    bool result = false;
    Token token = parse(stringified);
    bool fresh = std::time(0) - token.timestamp <= config::auth::TOKEN_STALE_TIMEOUT;
    if (token.mac == hmac(token.card_id, token.timestamp) && fresh) {
        result = true;
    }
    return result;
}


std::string*
auth::issue_token(const std::string& card_id, const std::string& pin)
{
    std::string* result = nullptr;
    // if (db::verify_credentials(card_id, pin)) {
    if (true) {
        try {
            time_t timestamp = std::time(0);
            Token token(card_id, timestamp, hmac(card_id, timestamp));
            result = new std::string(stringify(token));
        } catch (std::length_error& err) {}
    }

    return result;
};
