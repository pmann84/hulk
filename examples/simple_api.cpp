#include "hulk/hulk.hpp"

// Log examples
// hulk::log::error("Some error message with arg: {}", 1);
// hulk::log::warn("Easy padding in numbers like {:08d}", 12);
// hulk::log::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
// hulk::log::info("Support for floats {:03.2f}", 1.23456);
// hulk::log::debug("This message should be displayed..");

int main()
{    
    // Flesh out the API
    hulk::app app;
    app.debug().port(5000);

    // app.port(8080)
    //    .multithreaded();

    // // Start by supporting simple function calls, lambda, function ptr, bound class member call
    app.route("/api", 
        [](const hulk::http::request& request)
        {
            switch (request.method)
            {
                case hulk::HttpMethod::Get:
                {
                    return hulk::http::response::ok("<html><h1>Hello, World!</h1></html>");
                }
                case hulk::HttpMethod::Post:
                {
                    return hulk::http::response(201);
                }
            }
            return hulk::http::response::ok();
        }
    );
    // app.route("hello/<int:id>", &routing_func);

    app.run();

    return 0;
}