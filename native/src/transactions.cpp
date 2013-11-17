/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Transactions
 *
 */


#include "transactions.h"
#include "db.h"
#include "auth.h"

#include <string>


transactions::Balance::Balance(double _available, double _hold):
    available(_available),
    hold(_hold)
{
    return;
}


transactions::Balance
transactions::balance_inquiry(const std::string& token)
{
    std::string card_id = auth::verify_token(token);
    double available = db::get_available_funds(card_id);
    double hold = db::get_on_hold_funds(card_id);
    return Balance(available, hold);
};
