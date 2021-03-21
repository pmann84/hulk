#include <hulk.hpp>

#include <sstream>

int main()
{    
    // Flesh out the API
    hulk::app app;
    app.debug().port(5000);

    // Set up routes
    app.route("/api/thing/<int>", 
        [](const hulk::http::request& request)
        {
            switch (request.method)
            {
                case hulk::HttpMethod::Get:
                {
                    std::stringstream html_ss;
                    html_ss << "<html><h1>Hello, World!</h1>" << "<p>Recieved parameters:";
                    html_ss << "<ul>";
                    // for (auto& param : url.params)
                    // {
                    //     html_ss << "<li>" << param << "</li>"
                    // }
                    html_ss << "</ul></p></html>";
                    return hulk::http::response::ok("<html><h1>Hello, World!</h1></html>");
                }
            }
            return hulk::http::response::not_found();
        }
    );

    app.run();

    return 0;
}