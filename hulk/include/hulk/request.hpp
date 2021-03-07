#ifndef HULK_REQUEST_H_
#define HULK_REQUEST_H_

#include "constants.hpp"
#include "http_headers.hpp"

#include <sstream>

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
            http_headers headers;
            body body;

            void populate_status_from_stream(std::stringstream& stream)
            {
                std::string method_str;
                stream >> method_str;
                method = method_from_string(method_str); // HttpMethod
                stream >> target; // String
                std::string version_str;
                stream >> version_str;
                version = version_from_string(version_str); // HttpVersion
            }

            void populate_headers_from_stream(std::stringstream& stream)
            {
                std::string key_str, val_str;
                stream >> key_str >> val_str;
                headers.add(key_str, val_str);
            }

            std::string to_string()
            {
                std::stringstream req;
                req << method << " " << target << " " << version << "\r\n";
                return req.str();
            }
        };
    }
}

#endif // HULK_REQUEST_H_