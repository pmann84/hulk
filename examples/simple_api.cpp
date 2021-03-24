#include <hulk.hpp>

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
                    return hulk::http::response::ok((std::string)"<html><h1>Hello, World!</h1></html>");
                }
            }
            return hulk::http::response::not_found();
        }
    );

    app.run();

    return 0;
}