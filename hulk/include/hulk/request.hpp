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
                if (key_str.empty() || val_str.empty())
                {
                    return;
                }
                // std::isspace(static_cast<unsigned char>(key_str[0]));
                key_str = strings::trim_right(key_str, ':');
                headers.add(key_str, val_str);
            }

            std::string to_string() const
            {
                std::stringstream req;
                req << method << " " << target << " " << version << "\r\n";
                for (auto &[key, value] : headers.headers())
                {
                    req << key << ": " << value << "\r\n";
                }
                return req.str();
            }
        };
    }
}

#endif // HULK_REQUEST_H_