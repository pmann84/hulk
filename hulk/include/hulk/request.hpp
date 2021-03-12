#ifndef HULK_REQUEST_H_
#define HULK_REQUEST_H_

#include "constants.hpp"
#include "http_headers.hpp"
#include "http_body.hpp"
#include "helpers.hpp"

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

            std::string to_string() const
            {
                std::stringstream req;
                req << method << " " << target << " " << version << "\r\n";
                for (auto &[key, value] : headers.headers())
                {
                    req << key << ": " << value << "\r\n";
                }
                req << body.data();
                return req.str();
            }
        };
    }
}

#endif // HULK_REQUEST_H_