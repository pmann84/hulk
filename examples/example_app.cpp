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


asio::streambuf message_buffer_;

void read(asio::ip::tcp::socket& socket)
{
   //std::string message_buffer;

   //boost::asio::async_read_until(socket_, response_, "\r\n",
   //   boost::bind(&client::handle_read_status_line, this,
   //      boost::asio::placeholders::error));
   asio::async_read_until(socket, message_buffer_, "\r\n",
      [](std::error_code error_code, size_t length)
      {
         if (!error_code)
         {
            hulk::log::info("Reading message (size {})...", length);

            size_t nBufferSize = asio::buffer_size(message_buffer_.data());
            std::stringstream ssOut;
            auto constBuffer = message_buffer_.data();
            std::copy(
               asio::buffers_begin(constBuffer),
               asio::buffers_begin(constBuffer) + nBufferSize,
               std::ostream_iterator<char>(ssOut)
            );
            hulk::log::info("Message recieved: {}", ssOut.str());
            message_buffer_.consume(nBufferSize);
            //std::cout << ssOut.str() << "\n";



            //message_buffer_.data();
            //std::istream response_stream(&message_buffer_);
            //std::string output;
            //response_stream >> output;
            // unsigned int status_code;
            // response_stream >> status_code;
            // std::string status_message;
            // std::getline(response_stream, status_message);
         }
         else
         {
            hulk::log::error("Error reading message: {}", error_code.message());
         }
      }
   );

   //hulk::log::info("Message received: {}", message_buffer.data());

    //asio::async_read(socket, asio::buffer(&message_buffer, 100),
    //    [&socket, &message_buffer](std::error_code error_code, size_t length)
    //    {
    //        if (!error_code)
    //        {
    //            hulk::log::info("Recieved message: {}", message_buffer);
    //            // Do something with the message, parse it and send it off to the route handler
    //            // Re task context with waiting on the socket
    //            //wait_for_message(); 
    //        }
    //        else
    //        {
    //            hulk::log::error("Error reading incoming message: {}", error_code.message());
    //            socket.close();
    //        }
    //    }
    //);
}



void send_response(asio::ip::tcp::socket& socket, hulk::http::response response)
{
    if (socket.is_open())
    {
        std::string msg = response.to_string();
        asio::async_write(socket, asio::buffer(&msg, msg.size()),
            [&socket](std::error_code error_code, size_t length)
            {
                if (!error_code)
                {
                    hulk::log::info("Sending response status {} - {}", 200, "OK");
                }
                else
                {
                    hulk::log::error("Error sending message: {}", error_code.message());
                    socket.close();
                }
            }
        );
    }
    else
    {
        hulk::log::error("Error sending message: socket is not open!");
    }
}

void send(asio::io_context& context, asio::ip::tcp::socket& socket, hulk::http::response response)
{
    asio::post(context, 
            [&socket, response]()
            {
                send_response(socket, response);
            }
        );
}

// ASYNC
void wait_for_message(asio::ip::tcp::acceptor& acceptor, asio::io_context& context)
{
    acceptor.async_accept(
        [&acceptor, &context](std::error_code error_code, asio::ip::tcp::socket socket)
        {
            if (!error_code)
            {
                // OK!
                hulk::log::info("New message on {}:{}", socket.remote_endpoint().address().to_string(), socket.remote_endpoint().port());

                // TODO: Validate connections here?
                // TODO: Read messages off the socket here
                read(socket);
                // TODO: Execute route and send response
                // TEMP: Send ok response for now until the rest of the architecture is in
                //hulk::http::response response;
                //send(context, socket, response);
            }
            else
            {
                hulk::log::error("Errored while trying to accept new connections. {}", error_code.message());
            }

            wait_for_message(acceptor, context);
        }
    );
}

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
    // asio::ip::tcp::socket socket(ctx);
    
    // acceptor.accept(socket);
    // First assign some work for the context to do so it doesnt shut down immediately on startup
    wait_for_message(acceptor, ctx);

    // Startup  - first start the context
    std::thread thread_ctx;
    thread_ctx = std::thread([&ctx](){ ctx.run(); });
    // hulk::log::info("Server up and running on endpoint {}:{}", ep.address().to_string(), ep.port());
    hulk::log::info("Server started successfully!");

    while(true)
    {
        // Process the messages here
    }

    // Shut down gracefully
    ctx.stop();
    if (thread_ctx.joinable())
    {
        thread_ctx.join();
    }
    hulk::log::info("Server Shutdown. Bye!");

    // Flesh out the API
    // hulk::app app;

    // app.port(8080)
    //    .multithreaded();

    // // Start by supporting simple function calls, lambda, function ptr, bound class member call
    // app.route("api", [](){});
    // app.route("hello/<int:id>", &routing_func);


    // app.run();

    return 0;
}