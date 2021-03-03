#ifndef HULK_HTTP_REQUEST_H_
#define HULK_HTTP_REQUEST_H_

#include "http_constants.hpp"

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
            header header;
            body body;

            static request parse(std::istream& input)
            {
                std::string output;
                input >> output;
            }
        };
    }
}

#endif // HULK_HTTP_REQUEST_H_