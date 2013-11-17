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

#include <iostream>

#include <string>
#include <sstream>
#include <exception>
#include <cassert>
#include <ctime>
using namespace std;

#include <openssl/hmac.h>


// Wrapper for OpenSSL HMAC
string hmac(const string& card_id, time_t timestamp)
{
    // Concatenating card_id and timestamp
    stringstream ss;
    ss << card_id << timestamp;

    // Converting sstream to unsigned char*
    const string& t = ss.str();
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
    Token(const string& card_id,
          time_t timestamp,
          const string& mac);
    explicit Token(const string& stringified);
    ~Token() {};

    const string card_id;
    const time_t timestamp;
    const string mac;
};


Token::Token(const string& _card_id,
             time_t _timestamp,
             const string& _mac):
    card_id(_card_id),
    timestamp(_timestamp),
    mac(_mac)
{
    return;
};


Token parse(const string& stringified)
{
    assert (stringified.length() <= 100);

    istringstream ss(stringified);
    char delimiter = config::auth::TOKEN_DELIMITER;

    string card_id;
    getline(ss, card_id, delimiter);

    string raw_timestamp;
    getline(ss, raw_timestamp, delimiter);
    stringstream ss1; ss1 << raw_timestamp;
    time_t timestamp;
    ss1 >> timestamp;

    string mac;
    getline(ss, mac, delimiter);

    return Token(card_id, timestamp, mac);
};


string stringify(const Token& token)
{
    stringstream ss;
    ss << token.card_id << config::auth::TOKEN_DELIMITER \
       << token.timestamp << config::auth::TOKEN_DELIMITER \
       << token.mac;
    return ss.str();
};


struct auth::stale_token: exception
{
    virtual const char* what() const noexcept {
        return "Stale token";
    }
};


struct auth::invalid_timestamp: exception
{
    virtual const char* what() const noexcept {
        return "Invalid timestamp";
    }
};


struct auth::bad_signature: exception
{
    virtual const char* what() const noexcept {
        return "Bad signature";
    }
};


string auth::process_token(const string& stringified)
{
    Token token = parse(stringified);
    time_t dtime = time(0) - token.timestamp;
    if (dtime < 0) {
        throw invalid_timestamp();
    } else if (dtime >= config::auth::TOKEN_STALE_TIMEOUT) {
        throw stale_token();
    } else if (token.mac != hmac(token.card_id, token.timestamp)) {
        throw bad_signature();
    }

    return token.card_id;
}


string auth::issue_token(const string& card_id, const string& pin)
{
    db::verify_credentials(card_id, pin);
    time_t timestamp = time(0);
    Token token(card_id, timestamp, hmac(card_id, timestamp));
    return stringify(token);
};
