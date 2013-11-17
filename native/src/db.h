/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Database manager
 *
 */


#pragma once

#include <string>


namespace db
{
    bool
    verify_credentials(const std::string& card_id, const std::string& pin);

    double
    get_available_funds(const std::string& card_id);

    double
    get_on_hold_funds(const std::string& card_id);
}
