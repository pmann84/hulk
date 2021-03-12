#ifndef HULK_HTTP_PARSER_H_
#define HULK_HTTP_PARSER_H_

#include "request.hpp"
#include "constants.hpp"

namespace hulk
{
    namespace parser
    {
        enum class status
        {
            completed,
            incomplete,
            bad_request,
        };

        enum class read_status
        {
            request_status,
            request_header,
            request_body,
        };
    }

    class http_parser
    {
    public:
        http_parser(http::request& req) 
            : m_request(req)
            , m_read_status(parser::read_status::request_status)
            , m_status(parser::status::incomplete)
        {
        }

        parser::status status() const
        {
            return m_status;
        }

        parser::read_status read_status() const
        {
            return m_read_status;
        }

        http_parser& operator<<(std::string data)
        {
            switch (m_read_status)
            {
                case parser::read_status::request_status:
                {
                    log::debug("Parsing request status from message data ({})...", data);
                    m_read_status = parse_status(data);
                    m_status = parser::status::incomplete;
                    break;
                }
                case parser::read_status::request_header:
                {
                    log::debug("Parsing request header from message data ({})...", data);
                    m_read_status = parse_header(data);
                    if (m_read_status == parser::read_status::request_body)
                    {
                        auto [success, header_value] = m_request.headers.get("Content-Length");
                        if ((success && header_value == "0") || !success)
                        {
                            log::debug("Finished reading message headers, and not expecting body. Complete message received!");
                            m_status = parser::status::completed;
                        }
                        else
                        {
                            log::debug("Finished reading message headers, expecting content of length {}. Proceed to read body...", header_value);
                            m_status = parser::status::incomplete;
                        }
                    }
                    break;
                }
                case parser::read_status::request_body:
                {
                    log::debug("Parsing request body from message data ({})...", data);
                    parse_body(data);
                    m_status = parser::status::completed;
                    break;
                }
            }
            return *this;
        }

    private:
        parser::read_status parse_status(std::string& status_string)
        {
            std::stringstream ss;
            ss << status_string;

            std::string method_str;
            ss >> method_str;
            m_request.method = method_from_string(method_str); // HttpMethod
            std::string target_str;
            ss >> target_str; // String
            m_request.target = target_str;
            std::string version_str;
            ss >> version_str;
            m_request.version = version_from_string(version_str); // HttpVersion

            return parser::read_status::request_header;
        }

        parser::read_status parse_header(std::string& header_string)
        {
            std::stringstream ss;
            ss << header_string;

            std::string key_str, val_str;
            ss >> key_str >> val_str;
            if (header_string == CRLF || key_str.empty() || val_str.empty())
            {
                return parser::read_status::request_body;
            }
            key_str = strings::trim_right(key_str, ':');
            m_request.headers.add(key_str, val_str);
            return parser::read_status::request_header;
        }

        parser::read_status parse_body(std::string& body_string)
        {
            m_request.body << body_string;
            return parser::read_status::request_body;
        }

    private:
        http::request& m_request;
        parser::read_status m_read_status;
        parser::status m_status;
    };
} // namespace hulk

#endif // HULK_HTTP_PARSER_H_
