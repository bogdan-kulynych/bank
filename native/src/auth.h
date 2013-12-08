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

    struct stale_token: auth::exception
    {
        virtual const char* what() const noexcept {
            return "Stale token";
        }
    };

    struct invalid_timestamp: auth::exception
    {
        virtual const char* what() const noexcept {
            return "Invalid timestamp";
        }
    };

    struct bad_signature: auth::exception
    {
        virtual const char* what() const noexcept {
            return "Bad signature";
        }
    };

    struct invalid_credentials: auth::exception
    {
        virtual const char* what() const noexcept {
            return "Invalid credentials";
        }
    };

}
