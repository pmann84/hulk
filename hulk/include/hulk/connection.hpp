#ifndef HULK_CONNECTION_H_
#define HULK_CONNECTION_H_

#include "logging.hpp"
#include "request.hpp"
#include "response.hpp"
#include "constants.hpp"
#include "iconnection_manager.hpp"

#include <asio.hpp>
#include <memory>

namespace hulk
{
    // This class is responsible for handling a single HTTP request, reading it off the socket, 
    // parsing it, and then executing the handler
    class connection : public std::enable_shared_from_this<connection>
    {
    public:
        connection(asio::ip::tcp::socket socket, iconnection_manager& conn_manager) 
            : m_socket(std::move(socket))
            , m_connection_manager(conn_manager)
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
            read_request_status();
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

        void read_body()
        {
            log::debug("Reading message body...");
            asio::async_read_until(m_socket, m_message_buffer, std::string("\r\n"),
                [this](std::error_code ec, std::size_t bytes_transferred)
                {
                    size_t buffer_size = asio::buffer_size(m_message_buffer.data());
                    log::debug("Read {} bytes. Current buffer size = {}", bytes_transferred, buffer_size);
                    // Also do routing here
                }
            );       
        }

        void read_header()
        {
            log::debug("Reading message header...");
            asio::async_read_until(m_socket, m_message_buffer, std::string("\r\n"),
                [this](std::error_code ec, std::size_t bytes_transferred)
                {
                    size_t buffer_size = asio::buffer_size(m_message_buffer.data());
                    log::debug("Read {} header bytes. Current buffer size = {}", bytes_transferred, buffer_size);
                    std::stringstream ss;
                    copy_bytes_from_buffer(ss, m_message_buffer, bytes_transferred);
                    m_request.populate_headers_from_stream(ss);
                    if (buffer_size - bytes_transferred == 0) // Tests whether there is anything more to read after this
                    {
                        // Finished reading header so now we decide what we need to do
                        // Check the content-length, if its zero then ther is no body and we are done reading
                        // Now route the message and execute the appropriate handler
                        auto [success, header_value] = m_request.headers.get("content-length");
                        if ((success && header_value == "0") || !success)
                        {
                            // ROUTING
                            log::info("Recieved message: \n{}", m_request.to_string());
                            log::info("Executing route {} handler {}", m_request.target, "HANDLER HERE");
                            // For now just return a canned response
                            http::response response;
                            response.body.add_body_data("<html><h1>Hello, World!</h1></html>", "text/html");
                            send(response);
                        }
                        else
                        {
                            // Otherwise read the body
                            read_body();
                        }
                    }
                    else
                    {
                        read_header(); // Continue to read the header
                    }
                }
            );
        }

        void read_request_status()
        {
            log::debug("Reading request status line...");
            asio::async_read_until(m_socket, m_message_buffer, std::string("\r\n"), 
                [this](std::error_code ec, std::size_t bytes_transferred)
                {
                    if (!ec)
                    {
                        // Copy the bytes from the socket into the message buffer
                        size_t buffer_size = asio::buffer_size(m_message_buffer.data());
                        log::debug("Read {} status bytes. Current buffer size = {}", bytes_transferred, buffer_size);
                        std::stringstream ss;
                        copy_bytes_from_buffer(ss, m_message_buffer, bytes_transferred);
                        // Read from the message buffer into the request
                        m_request.populate_status_from_stream(ss);
                        // Re-prime the context to read the next part of the message
                        read_header();
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
        iconnection_manager& m_connection_manager;
    };
}

#endif // HULK_CONNECTION_H_