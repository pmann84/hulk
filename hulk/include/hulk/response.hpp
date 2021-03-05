#ifndef HULK_RESPONSE_H_
#define HULK_RESPONSE_H_

#include "constants.hpp"

#include <sstream>

namespace hulk
{
    using header_t = std::map<std::string, std::string>;

    namespace http
    {
        struct response
        {
            HttpVersion version = HttpVersion::Http11;
            uint32_t status = 200;
            header_t headers;

            std::string to_string()
            {
                std::stringstream ss;
                ss << version << " " << status << " " << StatusCodeMap[status] << "\r\n";
                return ss.str();
            }
        };
    }
}

#endif // HULK_RESPONSE_H_