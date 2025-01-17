#include <hulk.hpp>

#include <sstream>

int main()
{    
    // Flesh out the API
    hulk::app app;
    app.debug().port(5000);

    // Set up routes
    app.route("/api/thing/<int:id>",
        [](const hulk::http::request& request, const hulk::url_parameters& url_params)
        {
            switch (request.method)
            {
                case hulk::HttpMethod::Get:
                {
                    std::stringstream html_ss;
                    html_ss << "<html><h1>Hello, World!</h1>";
                    if (request.query_parameters.size() > 0)
                    {
                        html_ss << "<p>Query parameters: <ul>";
                        for (auto& [key, val] : request.query_parameters.headers())
                        {
                            html_ss << "<li>" << key << " - " << val  << "</li>";
                        }
                        html_ss << "</ul></p>";
                    }
                    html_ss << "<p>URL parameters: <ul>";
                    html_ss << "<li>" << "id" << " - " << std::get<uint64_t>(url_params.at("id"))  << "</li>";
                    html_ss << "</ul></p>";
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