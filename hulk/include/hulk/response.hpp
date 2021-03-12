#ifndef HULK_RESPONSE_H_
#define HULK_RESPONSE_H_

#include "constants.hpp"
#include "http_headers.hpp"
#include "http_body.hpp"
#include <sstream>
#include <ctime>
#include <time.h>
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
            
            response(uint32_t status) : status(status)
            {
                add_date_header();
                // add_header("Connection", "close");
            }

            void add_header(std::string key, std::string value)
            {
                headers.add(key, value);
            }

            void add_date_header()
            {
                // Date: Mon, 27 Jul 2009 12:28:53 GMT
                std::time_t t = std::time(nullptr);
                std::tm tm;
                gmtime_s(&tm, &t);
                std::stringstream datetime_ss;
                datetime_ss << std::put_time(&tm, "%a, %d %b %Y %T %Z");
                add_header("Date", datetime_ss.str());
            }

            void add_server_header(std::string server)
            {
                add_header("Server", server);
            }

            void add_content_length_header(uint64_t length)
            {
                add_header("Content-Length", std::to_string(length));
            }

            void add_content_type_header(std::string type)
            {

                add_header("Content-Type", type);
            }

            void add_content_header()
            {
                if(body.empty())
                {
                    add_content_length_header(0);
                }
                else
                {
                    add_content_length_header(body.content_length());
                    add_content_type_header(body.content_type());
                }
            }

            std::string to_string()
            {
                add_content_header();
                
                std::stringstream ss;
                ss << version << " " << status << " " << StatusCodeMap[status] << CRLF;
                for (auto &[key, value] : headers.headers())
                {
                    ss << key << ": " << value << CRLF;
                }
                // Output body
                if (!body.empty())
                {
                    ss << CRLF;
                    ss << body.data();
                }
                ss << CRLF;
                return ss.str();
            }

            static http::response ok()
            {
                return http::response(200);
            }

            static http::response ok(std::string body)
            {
                http::response response_with_body(200);
                response_with_body.body << body;
                response_with_body.body.content_type("text/html");
                return response_with_body;
            }

            static http::response not_found()
            {
                return http::response(404);
            }
        };
    } // namespace http
} // namespace hulk

#endif // HULK_RESPONSE_H_