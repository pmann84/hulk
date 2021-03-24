#ifndef HULK_REQUEST_H_
#define HULK_REQUEST_H_

#include "constants.hpp"
#include "parameter_map.hpp"
#include "http_body.hpp"

#include <sstream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <functional>

namespace hulk
{
    namespace http
    {
        struct request
        {
            HttpMethod method; // Method on the request
            std::string target; // Target of the request i.e. route requested etc, 
            HttpVersion version;
            http_headers headers;
            http_body body;
            query_parameters query_parameters;

            explicit operator std::string() const
            {
                std::stringstream req;
                req << method << " " << target << " " << version << CRLF;
                for (auto &[key, value] : headers.headers())
                {
                    req << key << ": " << value << CRLF;
                }
                if (!body.empty())
                {
                    req << CRLF << body.data();
                }
                return req.str();
            }
        };
    }
}

#endif // HULK_REQUEST_H_