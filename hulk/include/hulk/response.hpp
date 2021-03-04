#ifndef HULK_RESPONSE_H_
#define HULK_RESPONSE_H_

#include "constants.hpp"

#include <sstream>

namespace hulk
{
    namespace http
    {
        struct response
        {
            std::string data = "HTTP/1.1 200 OK";
            // HttpVersion version;
            // HttpStatusCode status;
            // std::string message;

            std::string to_string()
            {
                return data;
                // std::stringstream ss;
                // ss << "HTTP/1.1 200 OK" << std::endl;
                // return ss.str();
            }
        };
    }
}

#endif // HULK_RESPONSE_H_