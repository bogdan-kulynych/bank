#pragma once

#include <string>


namespace utils
{
    namespace base64
    {
        // Throws std::length_error when input too long
        std::string
        encode(const unsigned char* bindata, size_t binlen);

        // Throws std::invalid_argument when encountered invalid chars
        std::string
        decode(const std::string &ascdata);
    }
}
