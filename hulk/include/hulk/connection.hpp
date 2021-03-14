#ifndef HULK_CONNECTION_H_
#define HULK_CONNECTION_H_

#include "logging.hpp"
#include "request.hpp"
#include "response.hpp"
#include "constants.hpp"
#include "iconnection_manager.hpp"
#include "router.hpp"
#include "http_parser.hpp"

#include <asio.hpp>
#include <memory>

namespace hulk
{
    // This class is responsible for handling a single HTTP request, reading it off the socket, 
    // parsing it, and then executing the handler
    class connection : public std::enable_shared_from_this<connection>
    {
    public:
        connection(asio::ip::tcp::socket socket, iconnection_manager& conn_manager, router& router) 
            : m_socket(std::move(socket))
            , m_parser(m_request)
            , m_connection_manager(conn_manager)
            , m_router(router)

        {
            log::debug("Creating new connection to handle request from endpoint {}", socket_to_string(m_socket));
        }

        ~connection()
        {
            log::debug("Disposing connection for endpoint {}", socket_to_string(m_socket));
            stop();
        }

        void start()
        {
            receive();
        }

        void stop()
        {
            m_socket.close();
        }

        const http::request& request() const
        {
            return m_request;
        }

    private:    
        void execute_request()
        {
            // Query the router for the target and pass in the request
            auto route_handler = m_router.try_get(m_request.target);
            auto response = route_handler(m_request);
            send(response);
        }

        void copy_bytes_from_buffer(std::stringstream& ss, asio::streambuf& buffer, size_t num_bytes)
        {
            auto const_buffer = buffer.data();
            std::copy(
                asio::buffers_begin(const_buffer),
                asio::buffers_begin(const_buffer) + num_bytes,
                std::ostream_iterator<char>(ss)
            );
            m_message_buffer.consume(num_bytes);
        }

        void send(http::response response)
        {
            std::shared_ptr<std::string> response_str = std::make_shared<std::string>(response.to_string());
            asio::async_write(m_socket, asio::buffer(response_str->c_str(), response_str->length()), 
                [this, response_str](const std::error_code& ec, std::size_t bytes_sent)
                {
                    if (!ec)
                    {
                        log::info("Sending response:\nBytes Sent: {}\nResponse Bytes: {}\nResponse Data: {}", bytes_sent, response_str->length(), *response_str);
                        m_socket.shutdown(asio::ip::tcp::socket::shutdown_both);
                    }
                    else
                    {
                        log::error("Error sending response!");
                    }

                    if (ec != asio::error::operation_aborted)
                    {
                        m_connection_manager.stop(shared_from_this());
                    }
                }
            );
        }

        void parse_data_from_stream(size_t bytes_transferred)
        {
            log::debug("Reading {} bytes...", bytes_transferred);
            std::stringstream ss;
            copy_bytes_from_buffer(ss, m_message_buffer, bytes_transferred);
            log::debug("Message data: {}", ss.str());
            // Pass the data to the parser
            m_parser << ss.str();
            log::debug("BYTES LEFT IN BUFFER: {}", m_message_buffer.size());
            if (m_parser.status() == parser::status::completed)
            {
                log::info("Finished reading message:\n{}", m_request.to_string());
                // ROUTING
                log::info("Executing route {} handler...", m_request.target);
                // Execute request
                execute_request();
            }
            else if (m_parser.status() == parser::status::bad_request)
            {
                // Bad request - construct response with 400 then write 
                log::error("Bad request recieved.");
                send(http::response(400));
            }
            else
            {
                // Need more data
                log::debug("Complete message not recieved, waiting for more data...");
                if (m_parser.read_status() == parser::read_status::request_body)
                {
                    receive_body();
                }
                else
                {
                    receive();
                }
            } 
        }

        void receive_body()
        {
            log::debug("Reading body data off socket...");
            auto [success, content_length_str] = m_request.headers.get("Content-Length");
            size_t content_length = std::stoi(content_length_str);
            parse_data_from_stream(content_length);
        }
    
        void receive()
        {
            log::debug("Reading data off socket...");
            asio::async_read_until(m_socket, m_message_buffer, CRLF,
                [this](std::error_code ec, std::size_t bytes_transferred)
                {
                    if (!ec)
                    {
                        parse_data_from_stream(bytes_transferred);
                    }
                    else
                    {
                        log::error("Error reading request header: {}", ec.message());
                    }
                }
            );
        }


    private:
        asio::ip::tcp::socket m_socket;
        asio::streambuf m_message_buffer; // Messages are read asynchronously so we store the message here until its ready
        http::request m_request;
        http_parser m_parser;
        iconnection_manager& m_connection_manager;
        router m_router;
    };
}

#endif // HULK_CONNECTION_H_
