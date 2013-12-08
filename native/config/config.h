#pragma once

#include <string>


// Change this on production
namespace config
{
    // Authentication
    namespace auth
    {
        const std::string SECRET = "TOP SECRET DON'T LOOK";
        const char TOKEN_DELIMITER = ':';
        const unsigned int TOKEN_STALE_TIMEOUT = 180;
    }

    // Database
    namespace db
    {
    	const std::string DATABASE_NAME = "dev.db";
    }
}
