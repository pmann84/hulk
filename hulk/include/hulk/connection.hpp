#ifndef HULK_CONNECTION_H_
#define HULK_CONNECTION_H_

#include "logging.hpp"
#include "message_queue.hpp"
#include "constants.hpp"

#include <asio.hpp>

namespace hulk
{
    // This class is responsible for handling a single HTTP request, reading it off the socket, parsing it, and then executing the handler
    class connection
    {
    public:
        connection(asio::ip::tcp::socket socket) 
            : m_socket(std::move(socket))
        {
           log::debug("Creating new connection to handle incoming message.");
        }

        void start()
        {
            read_request_status();
        }

    private:
        void read_header()
        {
            log::debug("Reading message header...");
            asio::async_read_until(m_socket, m_message_buffer, '\r\n',
                [this](std::error_code ec, std::size_t bytes_transferred)
                {
                    size_t buffer_size = asio::buffer_size(m_message_buffer.data());
                    log::debug("Read {} bytes. Current buffer size = {}", bytes_transferred, buffer_size);
                    
                }
            );
        }

        void read_request_status()
        {
            log::debug("Reading request status line...");
            asio::async_read_until(m_socket, m_message_buffer, '\r\n', 
                [this](std::error_code ec, std::size_t bytes_transferred)
                {
                    // Copy the bytes from the socket into the message buffer
                    size_t buffer_size = asio::buffer_size(m_message_buffer.data());
                    log::debug("Read {} bytes. Current buffer size = {}", bytes_transferred, buffer_size);
                    std::stringstream ss;
                    auto const_buffer = m_message_buffer.data();
                    std::copy(
                        asio::buffers_begin(const_buffer),
                        asio::buffers_begin(const_buffer) + buffer_size,
                        std::ostream_iterator<char>(ss)
                    );
                    // Read from the message buffer into the request
                    std::string method_str;
                    ss >> method_str;
                    m_request.method = method_from_string(method_str); // HttpMethod
                    ss >> m_request.target; // String
                    std::string version_str;
                    ss >> version_str;
                    m_request.version = version_from_string(version_str); // HttpVersion
                    // Re-prime the context to read the next part of the message
                    read_header();
                }
            );



    //               asio::async_read_until(pThis->socket, pThis->buff, '\r', [pThis](const error_code& e, std::size_t s)
    //   {
    //      std::string line, ignore;
    //      std::istream stream {&pThis->buff};
    //      std::getline(stream, line, '\r');
    //      std::getline(stream, ignore, '\n');
    //      pThis->headers.on_read_request_line(line);
    //      pThis->read_next_line(pThis);
    //   });
        }

        // ASYNC
        // void read()
        // {
        //     // log::debug("ENTERED READ.");
        //     asio::async_read(m_socket, m_message_buffer,
        //         [this](std::error_code e, std::size_t length)
        //         {
        //             if (!e)
        //             {
        //                 size_t buffer_size = asio::buffer_size(m_message_buffer.data());
        //                 log::debug("Read {} bytes. Current buffer size = {}", length, buffer_size);
        //                 std::stringstream ss;
        //                 auto const_buffer = m_message_buffer.data();
        //                 std::copy(
        //                     asio::buffers_begin(const_buffer),
        //                     asio::buffers_begin(const_buffer) + buffer_size,
        //                     std::ostream_iterator<char>(ss)
        //                 );
        //                 hulk::log::info("Message recieved: {}", ss.str());
        //                 m_message_buffer.consume(buffer_size);
        //             }
        //             else
        //             {
        //                 log::error("Error reading message: {}", e.message());
        //                 m_socket.close();
        //             }
        //             read();
        //         }
        //     );
        // }

    private:
        asio::ip::tcp::socket m_socket;
        // message_queue& m_message_queue;
        asio::streambuf m_message_buffer; // Messages are read asynchronously so we store the message here until its ready
        http::request m_request;
    };
}

#endif // HULK_CONNECTION_H_