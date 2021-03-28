#include <hulk.hpp>

#include <sstream>

int main()
{    
    // Flesh out the API
    hulk::app app;
    app.port(5000);

    // Set up routes
    app.route("/api/thing/<int>",
        [](const hulk::http::request& request)
        {
            switch (request.method)
            {
                case hulk::HttpMethod::Get:
                {
                    std::stringstream html_ss;
                    html_ss << "<html><h1>Hello, World!</h1>";
                    if (request.query_parameters.size() > 0)
                    {
                        html_ss << "<p>Received parameters: <ul>";
                        for (auto& [key, val] : request.query_parameters.headers())
                        {
                            html_ss << "<li>" << key << " - " << val  << "</li>";
                        }
                        html_ss << "</ul></p>";
                    }
                    html_ss << "</html>";
                    return hulk::http::response::ok(html_ss.str());
                }
            }
            return hulk::http::response::not_found();
        }
    );

    app.run();

    return 0;
}