#ifndef HULK_RESPONSE_H_
#define HULK_RESPONSE_H_

#include "constants.hpp"

#include <sstream>
#include <ctime>
#include <iomanip>

namespace hulk
{
    // Access-Control-Allow-Origin: *
    // Connection: Keep-Alive
    // Content-Encoding: gzip
    // Content-Type: text/html; charset=utf-8
    // Etag: "c561c68d0ba92bbeb8b0f612a9199f722e3a621a"
    // Keep-Alive: timeout=5, max=997
    // Last-Modified: Mon, 18 Jul 2016 02:36:04 GMT
    // Set-Cookie: mykey=myvalue; expires=Mon, 17-Jul-2017 16:06:00 GMT; Max-Age=31449600; Path=/; secure
    // Transfer-Encoding: chunked
    // Vary: Cookie, Accept-Encoding
    // X-Backend-Server: developer2.webapp.scl3.mozilla.com
    // X-Cache-Info: not cacheable; meta data too large
    // X-kuma-revision: 1085259
    // x-frame-options: DENY

    namespace http
    {
        struct response
        {
            HttpVersion version = HttpVersion::Http11;
            uint32_t status = 200;
            header_t headers;

            void add_header(std::string key, std::string value)
            {
                headers[key] = value;
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
                std::stringstream ss;
                ss << version << " " << status << " " << StatusCodeMap[status] << "\r\n";
                for (auto &[key, value] : headers)
                {
                    ss << key << ": " << value << "\r\n";
                }
                return ss.str();
            }
        };
    }
}

#endif // HULK_RESPONSE_H_