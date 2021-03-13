#include "hulk/hulk.hpp"

int main()
{    
    // Flesh out the API
    hulk::app app;
    app.debug().port(5000);

    // Set up routes
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

    app.run();

    return 0;
}