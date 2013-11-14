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


namespace auth
{
    bool verify_token(const std::string& token);
    std::string* issue_token(const std::string& card_id, const std::string& pin);
}
