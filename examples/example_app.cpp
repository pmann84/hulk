// #include <asio.hpp>
// #include <SDKDDKVer.h>

#include <hulk/hulk.hpp>

void routing_func(int id)
{

};


// TODO: Response caching
// TODO: Query parameters e.g. localhost:8080/hello?p1=5&p2=world etc etc

// TODO: Think about how we might do this with a class
// TODO: Also think about how we might do url namespaces i.e. providing a prefix to a set of api end points
// e.g. api/hello and api/world so you dont have to specify the prefix "api"
// class iview
// {
// public:

// };

// class myview : public iview
// {
// public:
// private:
// };

#include <asio.hpp>

int main()
{
    hulk::log::init();
    // hulk::log::error("Some error message with arg: {}", 1);
    // hulk::log::warn("Easy padding in numbers like {:08d}", 12);
    // hulk::log::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    // hulk::log::info("Support for floats {:03.2f}", 1.23456);
    // hulk::log::debug("This message should be displayed..");

    uint32_t port = 8080;

    asio::io_context ctx; // IO Context
    asio::ip::tcp::endpoint ep(asio::ip::tcp::v4(), port);
    asio::ip::tcp::acceptor acceptor(ctx, ep); // Acceptor that listens on endpoint
    hulk::log::info("Running on endpoint {}:{}", ep.address().to_string(), ep.port());

    while(true)
    {
        asio::ip::tcp::socket socket(ctx);
        acceptor.accept(socket);
    }


    // hulk::app app;

    // app.port(8080)
    //    .multithreaded();

    // // Start by supporting simple function calls, lambda, function ptr, bound class member call
    // app.route("api", [](){});
    // app.route("hello/<int:id>", &routing_func);


    // app.run();

    return 0;
}