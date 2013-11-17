/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Database manager
 *
 */


#include "db.h"

#include <string>


bool
db::verify_credentials(const std::string& card_id, const std::string& pin)
{
    return true;
}


double
db::get_available_funds(const std::string& card_id)
{
    return 100500;
}


double
db::get_on_hold_funds(const std::string& card_id)
{
    return 9000;
}
