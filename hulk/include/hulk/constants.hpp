#ifndef HULK_HTTP_CONSTANTS_H_
#define HULK_HTTP_CONSTANTS_H_

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

    // HttpMethod parse(std::string method_str)
    // {
    //     switch (method_str)
    //     {
    //         case "GET":
    //             return HttpMethod::GET;
    //     }
    // }

    enum class HttpVersion
    {
        Http09,
        Http10,
        Http11,
        Http20,
    };

    enum class HttpStatusCode
    {
    };
}

#endif // HULK_HTTP_CONSTANTS_H_