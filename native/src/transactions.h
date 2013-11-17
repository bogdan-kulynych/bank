/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Transactions
 *
 */


#pragma once

#include <string>


namespace transactions
{
    struct Balance {
        Balance(double, double);

        double available;
        double hold;
    };

    Balance balance_inquiry(const std::string& token);
}
