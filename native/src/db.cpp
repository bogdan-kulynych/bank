/** 
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Database calls
 *
 */


#include "db.h"
#include "db/manager.h"
#include "config.h"
using namespace db;

#include <string>
#include <memory>
#include <exception>
#include <set>
using namespace std;


struct Connection {
    Manager *manager;

    Manager* operator->() {
        // If no connection established, establishes one
        if (!manager) {
            try {
                manager = new Manager();
                manager->openDB(config::db::DATABASE_NAME);
                return manager;

            } catch (sqlite_error& e) {
                throw database_error(e.what());
            }

        } else {
            return manager;
        }
    };
} conn;


bool
db::verify_credentials(const std::string& card_id, const std::string& pin)
{
    if (!exists(card_id)) {
        throw card_does_not_exist();
    } else {
        try {
            return conn->authRequest(card_id, pin);
        } catch (sqlite_error& e) {
            throw database_error(e.what());
        }
    }
}


bool db::exists(const std::string& card_id)
{
    try {
        return conn->existsRequest(card_id);
    } catch (sqlite_error& e) {
        throw database_error(e.what());
    }
}


void db::withdraw(const std::string& card_id,
        const double amount)
{
    if (amount <= 0) {
        throw invalid_amount();

    } else if (!exists(card_id)) {
        throw card_does_not_exist();

    } else {
        double available = db::get_available_funds(card_id);

        if (available < amount) {
            throw insufficient_funds();
        }

        try {
            return conn->changeAvailable(card_id, available-amount);
        } catch (sqlite_error& e) {
            throw database_error(e.what());
        }
    }
}


void db::make_transfer(const std::string& sender_id,
        const std::string& recepient_id,
        const double amount
    )
{
    if (amount <= 0) {
        throw invalid_amount();

    } else if (!exists(sender_id)) {
        throw card_does_not_exist();

    } else if (!exists(recepient_id)) {
        throw recepient_does_not_exist();

    } else {

        try {
            double sender_available = conn->availableRequest(sender_id);
            
            if (sender_available < amount) {
                throw insufficient_funds();
            }

            conn->changeAvailable(sender_id, sender_available - amount);

            double current_amount = amount;
            string current_recepient = recepient_id;
            
            // Walking overflow accounts chain
            do {
                double max_sum = conn->maxSumRequest(current_recepient);
                string supplementary = conn->supplementaryRequest(current_recepient);
                double recepient_available = conn->availableRequest(current_recepient);

                double new_sum = recepient_available + current_amount;
                if (supplementary != "") {
                    if (new_sum > max_sum) {
                         current_amount = new_sum - max_sum;
                         new_sum = max_sum;
                    } else {
                        current_amount = 0;
                    }
                }

                conn->changeAvailable(current_recepient, new_sum);
                current_recepient = supplementary;

            } while (current_amount > 0);

        } catch (sqlite_error& e) {
            throw database_error(e.what());
        }
    }
}


string db::get_name(const std::string& card_id)
{
    if (!exists(card_id)) {
        throw card_does_not_exist();
    } else {
        try {
            return conn->nameRequest(card_id);
            
        } catch (sqlite_error& e) {
            throw database_error(e.what());
        }
    }
}


double db::get_available_funds(const std::string& card_id)
{
    if (!exists(card_id)) {
        throw card_does_not_exist();
    } else {
        try {
            return conn->availableRequest(card_id);
        } catch (sqlite_error& e) {
            throw database_error(e.what());
        }
    }
}


double db::get_on_hold_funds(const std::string& card_id)
{
    if (!exists(card_id)) {
        throw card_does_not_exist();
    } else {
        try {
            return conn->holdRequest(card_id);
        } catch (sqlite_error& e) {
            throw database_error(e.what());
        }
    }
}


double
db::get_overflow_threshold(const string card_id)
{
    if (!exists(card_id)) {
        throw card_does_not_exist();
    } else {
        try {
            return conn->maxSumRequest(card_id);
        } catch (sqlite_error& e) {
            throw database_error(e.what());
        }
    }
}


string
db::get_overflow_recepient(const string card_id)
{
    if (!exists(card_id)) {
        throw card_does_not_exist();
    } else {
        try {
            return conn->supplementaryRequest(card_id);
        } catch (sqlite_error& e) {
            throw database_error(e.what());
        }
    }
}


void
db::set_overflow_threshold(const string card_id, double threshold) 
{
    if (threshold != 0 && (threshold < 0 || threshold < get_available_funds(card_id))) {
        throw invalid_amount();

    } else if (!exists(card_id)) {
        throw card_does_not_exist();

    } else {
        try {
            return conn->changeMaxSum(card_id, threshold);
        } catch (sqlite_error& e) {
            throw database_error(e.what());
        }
    }
}


void
db::set_overflow_recepient(const string card_id, const string recepient_id)
{
    if (!exists(card_id)) {
        throw card_does_not_exist();
    } else if (recepient_id != "" && !exists(recepient_id)) {
        throw recepient_does_not_exist();
    } else {
        try {
            // Check if there exists a loop
            if (recepient_id != "") {
                auto supplementary = conn->supplementaryRequest(recepient_id);
                while (supplementary != "") {
                    if (supplementary == card_id) {
                        throw conflict_supplementary_account();
                    }
                    supplementary = conn->supplementaryRequest(supplementary);
                }
            }

            return conn->changeSupplCard(card_id, recepient_id);

        } catch (sqlite_error& e) {
            throw database_error(e.what());
        }   
    }
}
