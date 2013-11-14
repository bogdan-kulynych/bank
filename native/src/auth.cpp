/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Authentication
 */

#include "db.h"
#include "auth.h"
#include "config.h"

#include "utils/base64.h"
using namespace utils;

#include <string>
#include <sstream>
#include <ctime>
#include <stdexcept>

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
    Token(const std::string& card_id, time_t timestamp=std::time(0));
    ~Token() {};

    const std::string card_id;
    const std::time_t timestamp;
    const std::string mac;
};


Token::Token(const std::string& _card_id, time_t _timestamp):
    card_id(_card_id),
    timestamp(_timestamp),
    mac(hmac(_card_id, _timestamp))
{
    return;
};


std::string stringify(const Token& token)
{
    std::stringstream ss;
    ss << token.card_id << config::auth::TOKEN_DELIMITER \
       << token.timestamp << config::auth::TOKEN_DELIMITER \
       << token.mac;
    return ss.str();
};


// TODO
Token* parse(const std::string& stringified)
{
    return nullptr;
};


// TODO
bool auth::verify_token(const std::string& token)
{
    return true;
}


std::string*
auth::issue_token(const std::string& card_id, const std::string& pin)
{
    std::string* result = nullptr;
    if (db::verify_credentials(card_id, pin)) {
        try {
            Token token(card_id, std::time(0));
            result = new std::string(stringify(token));
        } catch (std::length_error& err) {}
    }

    return result;
};
