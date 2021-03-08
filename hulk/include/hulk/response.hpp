#ifndef HULK_RESPONSE_H_
#define HULK_RESPONSE_H_

#include "constants.hpp"
#include "http_headers.hpp"
#include "http_body.hpp"
#include <sstream>
#include <ctime>
#include <iomanip>

namespace hulk
{
    namespace http
    {
        struct response
        {
            HttpVersion version = HttpVersion::Http11;
            uint32_t status = 200;
            http_headers headers;
            http_body body;
            
            response()
            {
                add_date_header();
                add_header("Connection", "close");
            }

            void add_header(std::string key, std::string value)
            {
                headers.add(key, value);
            }

            void add_date_header()
            {
                // Date: Mon, 27 Jul 2009 12:28:53 GMT
                std::time_t t = std::time(nullptr);
                std::stringstream datetime_ss;
                datetime_ss << std::put_time(std::gmtime(&t), "%a, %d %b %Y %T %Z");
                add_header("Date", datetime_ss.str());
            }

            void add_server_header(std::string server)
            {
                add_header("Server", server);
            }

            std::string to_string()
            {
                if(body.empty())
                {
                    add_header("Content-Length", "0");
                }
                
                std::stringstream ss;
                ss << version << " " << status << " " << StatusCodeMap[status] << "\r\n";
                for (auto &[key, value] : headers.headers())
                {
                    ss << key << ": " << value << "\r\n";
                }
                // Output body
                if (!body.empty())
                {
                    ss << body.content_length();
                    ss << body.content_type();
                    ss << "\r\n";
                    ss << body.data();
                }
                return ss.str();
            }
        };
    }
}

#endif // HULK_RESPONSE_H_