#include "hulk/hulk.hpp"

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
                hulk::json response = "{ \"Hello\",\"World\" }"_json;
                return hulk::http::response::ok(response);
            }
        }
    );

    app.run();

    return 0;
}