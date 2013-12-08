/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Transactions
 *
 */


#pragma once

#include <string>


namespace ops
{
    struct Balance {
        Balance(double, double);

        double available;
        double hold;
    };

    // Attempts to make balance inquiry. Can throw auth or db exception
    Balance balance_inquiry(const std::string& token);

    // Attempts to withdraw funds. Can throw auth or db exception
    void withdrawal_request(const std::string& token,
        double amount);

    // Attempts to make transfer. Can throw auth or db exception
    void transfer_request(const std::string& token,
        const std::string& recepient_id,
        double amount);

    // Gets name of card_id cardholder. Can throw auth or db exception
    std::string name_request(const std::string& token,
        const std::string& card_id);

    // Gets oveflow recepient and threshold
    std::string get_overflow_recepient(const std::string& token);
    double get_overflow_threshold(const std::string& token);

    // Sets oveflow recepient and threshold
    void set_overflow_recepient(const std::string& token, const std::string& recepient_id);
    void set_overflow_threshold(const std::string& token, const double threshold);
}
