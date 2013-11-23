/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Authentication
 * Functions to issue and verify auth tokens. Auth token is a string
 * `<card id>:<timestamp>:<MAC>` used as digital signature to
 * authenticate user session.
 */


#pragma once

#include <string>
#include <exception>


namespace auth
{
    // Returns card id if token is valid, otherwise throws auth exception
    // Does not verify if card id itself is valid, use db::exists
    std::string
    process_token(const std::string& token);

    // Returns an auth token if credentials are valid, otherwise
    // throws db exception
    std::string
    issue_token(const std::string& card_id, const std::string& pin);

    // Exceptions
    struct exception : public std::exception {};

    struct invalid_credentials;
    struct stale_token;
    struct invalid_timestamp;
    struct bad_signature;
}
