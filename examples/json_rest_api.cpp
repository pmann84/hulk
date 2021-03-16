#include <hulk.hpp>

int main()
{    
    // Flesh out the API
    hulk::app app;
    app.debug().port(5000);

    app.route("/api", 
        [](const hulk::http::request& request)
        {
            switch (request.method)
            {
                case hulk::HttpMethod::Get:
                { 
                    // hulk::json response = "{ \"Hello\",\"World\" }"_json;
                    hulk::json response;
                    response["Hello"] = "World";
                    return hulk::http::response::ok(response.dump(), "application/json");
                }
                default:
                {
                    return hulk::http::response::not_found();
                }
            }
        }
    );

    app.run();

    return 0;
}