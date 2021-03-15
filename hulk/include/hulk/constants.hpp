#ifndef HULK_HTTP_CONSTANTS_H_
#define HULK_HTTP_CONSTANTS_H_

#include <map>
#include <sstream>
#include <ostream>

#include <asio.hpp>

namespace hulk
{
    class parameter_map;

    static std::string CRLF = "\r\n";
    static std::string HTTP_DATETIME_FORMAT = "%a, %d %b %Y %T %Z";

    using http_headers = parameter_map;
    using query_parameters = parameter_map;

    std::string socket_to_string(asio::ip::tcp::socket& socket)
    {
        std::stringstream ss;
        ss << socket.remote_endpoint().address().to_string() << ":" << socket.remote_endpoint().port();
        return ss.str();
    }

    // https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
    enum class HttpMethod
    {
        Unknown, // Invalid http method
        Get    , // GET method requests a representation of the specified resource. Requests using GET should only retrieve data.
        Head   , // HEAD method asks for a response identical to that of a GET request, but without the response body.
        Post   , // POST method is used to submit an entity to the specified resource, often causing a change in state or side effects on the server.
        Put    , // PUT method replaces all current representations of the target resource with the request payload.
        Delete , // DELETE method deletes the specified resource.
        Connect, // CONNECT method establishes a tunnel to the server identified by the target resource.
        Options, // OPTIONS method is used to describe the communication options for the target resource.
        Trace  , // TRACE method performs a message loop-back test along the path to the target resource.
        Patch  , //  PATCH method is used to apply partial modifications to a resource.
    };

    hulk::HttpMethod method_from_string(std::string method)
    {
        if (method == "GET")
            return hulk::HttpMethod::Get;
        else if (method == "HEAD")
            return hulk::HttpMethod::Head;
        else if (method == "POST")
            return hulk::HttpMethod::Post;
        else if (method == "PUT")
            return hulk::HttpMethod::Put;
        else if (method == "DELETE")
            return hulk::HttpMethod::Delete;
        else if (method == "CONNECT")
            return hulk::HttpMethod::Connect;
        else if (method == "OPTIONS")
            return hulk::HttpMethod::Options;
        else if (method == "TRACE")
            return hulk::HttpMethod::Trace;
        else if (method == "PATCH")
            return hulk::HttpMethod::Patch;
        else
            return hulk::HttpMethod::Unknown;
    }

    enum class HttpVersion
    {
        Unknown,
        Http09,
        Http10,
        Http11,
        Http20,
    };

    HttpVersion version_from_string(std::string version)
    {
        if (version == "HTTP/0.9")
            return HttpVersion::Http09;
        else if (version == "HTTP/1.0")
            return HttpVersion::Http10;
        else if (version == "HTTP/1.1")
            return HttpVersion::Http11;
        else if (version == "HTTP/2.0")
            return HttpVersion::Http20;
        else
            return HttpVersion::Unknown;
    }

    static std::map<uint32_t, std::string> StatusCodeMap = {
        {100, "Continue"},
        {101, "Switching Protocols"},
        {103, "Early Hints"},
        {200, "OK"},
        {201, "Created"},
        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},
        {300, "Multiple Choices"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {303, "See Other"},
        {304, "Not Modified"},
        {307, "Temporary Redirect"},
        {308, "Permanent Redirect"},
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {408, "Request Timeout"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},
        {412, "Precondition Failed"},
        {413, "Payload Too Large"},
        {414, "URI Too Long"},
        {415, "Unsupported Media Type"},
        {416, "Range Not Satisfiable"},
        {417, "Expectation Failed"},
        {418, "I'm a teapot"},
        {422, "Unprocessable Entity"},
        {425, "Too Early"},
        {426, "Upgrade Required"},
        {428, "Precondition Required"},
        {429, "Too Many Requests"},
        {431, "Request Header Fields Too Large"},
        {451, "Unavailable For Legal Reasons"},
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {502, "Bad Gateway"},
        {503, "Service Unavailable"},
        {504, "Gateway Timeout"},
        {505, "HTTP Version Not Supported"},
        {506, "Variant Also Negotiates"},
        {507, "Insufficient Storage"},
        {508, "Loop Detected"},
        {510, "Not Extended"},
        {511, "Network Authentication Required"},
    };

    namespace content_type
    {
        enum class application
        {
            json,
            xml,
        };

        enum class text
        {
            css,
            csv,
            html,
            javascript,
            plain,
            xml,
        };

        enum class audio
        {
            // audio/mpeg
            // audio/x-ms-wma
            // audio/vnd.rn-realaudio
            // audio/x-wav
        };

        enum class image
        {
            // image/gif
            // image/jpeg
            // image/png
            // image/tiff
            // image/vnd.microsoft.icon
            // image/x-icon
            // image/vnd.djvu
            // image/svg+xml
        };

        enum class video
        {
            // video/mpeg
            // video/mp4
            // video/quicktime
            // video/x-ms-wmv
            // video/x-msvideo
            // video/x-flv
            // video/webm
        };
    }

    static std::map<content_type::application, std::string> ContentTypeApplicationMap = {
        {content_type::application::json, "application/json"},
        {content_type::application::xml, "application/xml"},
    };

    static std::map<content_type::text, std::string> ContentTypeMap = {
        {content_type::text::css, "text/css"},
        {content_type::text::csv, "text/csv"},
        {content_type::text::html, "text/html"},
        {content_type::text::javascript, "text/javascript"},
        {content_type::text::plain, "text/plain"},
        {content_type::text::xml, "text/xml"},
    };

    // Content headers
    // application/EDI-X12
    // application/EDIFACT
    // application/javascript
    // application/octet-stream
    // application/ogg
    // application/pdf
    // application/xhtml+xml
    // application/x-shockwave-flash
    // application/ld+json
    // application/xml
    // application/zip
    // application/x-www-form-urlencoded
    // Audio	

    // Image	

    // Multipart	
    // multipart/mixed
    // multipart/alternative
    // multipart/related (using by MHTML (HTML mail).)
    // multipart/form-data
    // Video	

    // VND	
    // application/vnd.oasis.opendocument.text
    // application/vnd.oasis.opendocument.spreadsheet
    // application/vnd.oasis.opendocument.presentation
    // application/vnd.oasis.opendocument.graphics
    // application/vnd.ms-excel
    // application/vnd.openxmlformats-officedocument.spreadsheetml.sheet
    // application/vnd.ms-powerpoint
    // application/vnd.openxmlformats-officedocument.presentationml.presentation
    // application/msword
    // application/vnd.openxmlformats-officedocument.wordprocessingml.document
    // application/vnd.mozilla.xul+xml
}

std::ostream& operator<<(std::ostream& os, hulk::HttpMethod method)
{
    switch (method)
    {
        case hulk::HttpMethod::Get:
        {
            os << "GET";
            break;
        }
        case hulk::HttpMethod::Head:
        {
            os << "HEAD";
            break;
        }
        case hulk::HttpMethod::Post:
        {
            os << "POST";
            break;
        }
        case hulk::HttpMethod::Put:
        {
            os << "PUT";
            break;
        }
        case hulk::HttpMethod::Delete:
        {
            os << "DELETE";
            break;
        }
        case hulk::HttpMethod::Connect:
        {
            os << "CONNECT";
            break;
        }
        case hulk::HttpMethod::Options:
        {
            os << "OPTIONS";
            break;
        }
        case hulk::HttpMethod::Trace:
        {
            os << "TRACE";
            break;
        }
        case hulk::HttpMethod::Patch:
        {
            os << "PATCH";
            break;
        }
        default:
        {
            std::stringstream ss;
            ss << "Cannot deserialise HTTP method: " << method; 
            hulk::log::error(ss.str());
            throw std::runtime_error(ss.str());
        }
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, hulk::HttpVersion version)
{
    switch (version)
    {
        case hulk::HttpVersion::Http09:
        {
            os << "HTTP/0.9";
            break;
        }
        case hulk::HttpVersion::Http10:
        {
            os << "HTTP/1.0";
            break;
        }
        case hulk::HttpVersion::Http11:
        {
            os << "HTTP/1.1";
            break;
        }
        case hulk::HttpVersion::Http20:
        {
            os << "HTTP/2.0";
            break;
        }
        default:
        {
            std::stringstream ss;
            ss << "Cannot deserialise HTTP version: " << version; 
            hulk::log::error(ss.str());
            throw std::runtime_error(ss.str());
        }
    }
    return os;
}

#endif // HULK_HTTP_CONSTANTS_H_