#ifndef HULK_REQUEST_H_
#define HULK_REQUEST_H_

#include "constants.hpp"

namespace hulk
{
    namespace http
    {
        struct header
        {

        };

        struct body
        {

        };

        struct request
        {
            HttpMethod method; // Method on the request
            std::string target; // Target of the request i.e. route requested etc, 
            HttpVersion version;
            header_t headers;
            body body;
        };
    }
}

#endif // HULK_REQUEST_H_