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
    // Returns card id if token is valid, otherwise throws exception
    std::string
    process_token(const std::string& token);

    // Returns an auth token if credentials are valid, otherwise
    // throws exception
    std::string
    issue_token(const std::string& card_id, const std::string& pin);

    // Exceptions
    struct stale_token;
    struct invalid_timestamp;
    struct bad_signature;
}
