#pragma once

#include <string>


namespace config
{
    // Authentication
    namespace auth
    {
        const std::string SECRET = "TOP SECRET DON'T LOOK";
        const char TOKEN_DELIMITER = ':';
        const unsigned int TOKEN_STALE_TIMEOUT = 180;
    }
}