#ifndef HULK_HTTP_CONSTANTS_H_
#define HULK_HTTP_CONSTANTS_H_

#include <map>
#include <string>
#include <ostream>

namespace hulk
{
    // https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
    enum class HttpMethod
    {
        Get     = 1 << 0, // GET method requests a representation of the specified resource. Requests using GET should only retrieve data.
        Head    = 1 << 1, // HEAD method asks for a response identical to that of a GET request, but without the response body.
        Post    = 1 << 2, // POST method is used to submit an entity to the specified resource, often causing a change in state or side effects on the server.
        Put     = 1 << 3, // PUT method replaces all current representations of the target resource with the request payload.
        Delete  = 1 << 4, // DELETE method deletes the specified resource.
        Connect = 1 << 5, // CONNECT method establishes a tunnel to the server identified by the target resource.
        Options = 1 << 6, // OPTIONS method is used to describe the communication options for the target resource.
        Trace   = 1 << 7, // TRACE method performs a message loop-back test along the path to the target resource.
        Patch   = 1 << 8, //  PATCH method is used to apply partial modifications to a resource.
    };

    std::ostream& operator<<(std::ostream& os, HttpMethod method)
    {
        switch (method)
        {
            case HttpMethod::Get:
            {
                os << "GET";
                break;
            }
            case HttpMethod::Head:
            {
                os << "HEAD";
                break;
            }
            case HttpMethod::Post:
            {
                os << "POST";
                break;
            }
            case HttpMethod::Put:
            {
                os << "PUT";
                break;
            }
            case HttpMethod::Delete:
            {
                os << "DELETE";
                break;
            }
            case HttpMethod::Connect:
            {
                os << "CONNECT";
                break;
            }
            case HttpMethod::Options:
            {
                os << "OPTIONS";
                break;
            }
            case HttpMethod::Trace:
            {
                os << "TRACE";
                break;
            }
            case HttpMethod::Patch:
            {
                os << "PATCH";
                break;
            }
            return os;
        }
    }

    enum class HttpVersion
    {
        Http09,
        Http10,
        Http11,
        Http20,
    };

    std::ostream& operator<<(std::ostream& os, HttpVersion version)
    {
        switch (version)
        {
            case HttpVersion::Http09:
            {
                os << "HTTP/0.9";
                break;
            }
            case HttpVersion::Http10:
            {
                os << "HTTP/1.0";
                break;
            }
            case HttpVersion::Http11:
            {
                os << "HTTP/1.1";
                break;
            }
            case HttpVersion::Http20:
            {
                os << "HTTP/2.0";
                break;
            }
            return os;
        }
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
}

#endif // HULK_HTTP_CONSTANTS_H_