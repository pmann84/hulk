#ifndef HULK_HTTP_PARSER_H_
#define HULK_HTTP_PARSER_H_

#include "request.hpp"
#include "constants.hpp"
#include "string_helpers.hpp"

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
                    m_status = validate_status(); // TODO: Can we just send the status line and no headers in a request?
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
        void parse_query_string(std::string query_string, http::request& request)
        {
            log::debug("Parsing query string: {}", query_string);
            std::vector<std::string> query_params = strings::split(query_string, std::string("&"));
            log::debug("{} query params detected...", query_params.size());
            if (query_params.size() == 0)
            {
                m_status = parser::status::bad_request; 
                return;
            }

            for (auto param : query_params)
            {
                std::vector<std::string> query_pair = strings::split(param, std::string("="));
                if (query_pair.size() != 2)
                {
                    m_status = parser::status::bad_request;
                    return;
                }
                else
                {
                    auto key = query_pair[0];
                    auto val = query_pair[1];
                    log::debug("Adding query parameter [{}: {}]", key, val);
                    m_request.query_parameters.add(key, val);
                }
            }
        }

        void parse_target(std::string& target_string, http::request& request)
        {
            // Here we parse query parameters that may have been supplied
            auto pos = target_string.find_first_of("?");
            // First check if we have any parameters
            if (pos == std::string::npos)
            {
                m_request.target = target_string;
            }
            else
            {
                m_request.target = target_string.substr(0, pos);
                std::string query_string = target_string.substr(pos + 1, target_string.size());
                parse_query_string(query_string, request);
            }
        }

        parser::read_status parse_status(std::string& status_string)
        {
            std::stringstream ss;
            ss << status_string;

            std::string method_str;
            ss >> method_str;
            m_request.method = method_from_string(method_str); // HttpMethod
            std::string target_str;
            ss >> target_str; // String
            parse_target(target_str, m_request);
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
                // Set the content type on the body too
                auto [success, content_type] = m_request.headers.get("Content-Type");
                m_request.body.content_type(content_type);
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

        parser::status validate_status()
        {
            if (m_status == parser::status::bad_request || m_request.version == HttpVersion::Unknown || m_request.method == HttpMethod::Unknown)
            {
                return parser::status::bad_request;
            }
            return parser::status::incomplete;
        }

    private:
        http::request& m_request;
        parser::read_status m_read_status;
        parser::status m_status;
    };
} // namespace hulk

#endif // HULK_HTTP_PARSER_H_
