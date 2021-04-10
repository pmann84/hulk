#include <hulk.hpp>

int main()
{    
    // Flesh out the API
    hulk::app app;
    app.port(5000);

    // Set up routes
    app.route("/api",
        [](const hulk::http::request& request, const hulk::url_parameters&)
        {
            switch (request.method)
            {
                case hulk::HttpMethod::Get:
                {
                    hulk::html_element doc;
                    doc.add("h1", "Hello, World!");
                    return hulk::http::response::ok(doc.to_string());
                }
            }
            return hulk::http::response::not_found();
        }
    );

    app.run();

    return 0;
}