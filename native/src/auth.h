/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Authentication
 * Functions to issue and verify auth tokens. Auth token is a tuple
 * (<card id>, <timestamp>, <MAC>) used as digital signature to
 * authenticate user session.
 */


#pragma once

#include <string>
#include <exception>


namespace auth
{
    // Throws exception if token is invalid, otherwise returns card id
    std::string verify_token(const std::string& token);

    // Returns a token for specified card id and pin, if invalid, throws exception
    std::string issue_token(const std::string& card_id, const std::string& pin);

    // Exceptions
    struct stale_token;
    struct invalid_timestamp;
    struct bad_signature;
}
