#pragma once

#include <string>
using namespace std;


namespace db
{
    bool verify_credentials(const std::string& card_id, const std::string& pin)
    {
        return true;
    }
}
