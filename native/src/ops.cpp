/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Transactions
 *
 */


#include "ops.h"
#include "db.h"
#include "auth.h"

#include <string>
using namespace std;


ops::Balance::Balance(double _available, double _hold):
    available(_available),
    hold(_hold)
{
    return;
}


ops::Balance
ops::balance_inquiry(const string& token)
{
    string card_id = auth::process_token(token);
    double available = db::get_available_funds(card_id);
    double hold = db::get_on_hold_funds(card_id);
    return Balance(available, hold);
}


void ops::withdrawal_request(const std::string& token, double amount)
{
    string card_id = auth::process_token(token);
    db::withdraw(card_id, amount);
}


void ops::transfer_request(const string& token,
    const string& recepient_id,
    double amount)
{
    string card_id = auth::process_token(token);
    db::make_transfer(card_id, recepient_id, amount);
}


string ops::name_request(const string& token,
    const string& card_id)
{
    auth::process_token(token);
    return db::get_name(card_id);
}


void ops::set_overflow_recepient(const string& token,
    const string& recepient_id,
    double threshold)
{
    string card_id = auth::process_token(token);
    db::set_overflow_recepient(card_id, recepient_id, threshold);
}
