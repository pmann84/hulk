#include <hulk.hpp>

#include <chrono>
#include <ctime>

template<typename T>
std::string get_date_string(std::chrono::time_point<T> t)
{
    auto as_time_t = std::chrono::system_clock::to_time_t(t);
    std::tm* as_tm = std::localtime(&as_time_t);
    char buffer[32];
    std::strftime(buffer, 32, "%d/%m/%Y %H:%M:%S", as_tm);
    return buffer;
}

int main()
{    
    // Flesh out the API
    hulk::app app;
    app.port(5000);

    app.route("/json",
              [](const hulk::http::request& request, const hulk::url_parameters&)
              {
                  switch (request.method)
                  {
                      case hulk::HttpMethod::Get:
                      {
                          hulk::json response;
                          response["message"] = "Hello, World!";
                          return hulk::http::response::ok(response.dump(), "application/json");
                      }
                      default:
                      {
                          return hulk::http::response::not_found();
                      }
                  }
              });

    app.route("/currenttime",
        [](const hulk::http::request& request, const hulk::url_parameters&)
        {
            switch (request.method)
            {
                case hulk::HttpMethod::Get:
                { 
                    hulk::json response;
                    response["time"] = get_date_string(std::chrono::system_clock::now());
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