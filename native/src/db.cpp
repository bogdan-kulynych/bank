/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Database manager
 *
 */


#include "db.h"

#include <string>
#include <exception>
using namespace std;


// TODO: Note, every function has to request database lock on start


bool
db::verify_credentials(const std::string& card_id, const std::string& pin)
{
    return true;
}


bool db::exists(const std::string& card_id)
{
    return true;
}


void db::withdraw(const std::string& card_id,
        const double amount)
{
    if (amount < 0) {
        throw invalid_amount();

    } else if (!exists(card_id)) {
        throw card_does_not_exist();
    } else if (db::get_available_funds(card_id) < amount) {
        throw insufficient_funds();
    }

    // actually decrease funds here
    return;
}


void db::make_transfer(const std::string& sender_id,
        const std::string& recepient_id,
        const double amount
    )
{
    if (amount < 0) {
        throw invalid_amount();

    } else if (!exists(sender_id)) {
        throw card_does_not_exist();

    } else if (!exists(recepient_id)) {
        throw recepient_does_not_exist();

    } else if (db::get_available_funds(sender_id) < amount) {
        throw insufficient_funds();
    }

    // actually make transfer here
    return;
}


string db::get_name(const std::string& card_id)
{
    return "John Doe";
}


double db::get_available_funds(const std::string& card_id)
{
    return 100500;
}


double db::get_on_hold_funds(const std::string& card_id)
{
    return 9000;
}


void
db::set_overflow_recepient(const std::string sender_id,
    const std::string recepient_id,
    double threshold)
{
    if (threshold < 0) {
        throw invalid_amount();

    } else if (!exists(sender_id)) {
        throw card_does_not_exist();

    } else if (!exists(recepient_id)) {
        throw recepient_does_not_exist();

    } else {

    }
}
