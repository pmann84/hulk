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
            asio::async_read_until(m_socket, m_message_buffer, '\r\n',
                [this](std::error_code ec, std::size_t bytes_transferred)
                {
                    size_t buffer_size = asio::buffer_size(m_message_buffer.data());
                    log::debug("Read {} bytes. Current buffer size = {}", bytes_transferred, buffer_size);
                    
                }
            );       
        }

        void read_header()
        {
            log::debug("Reading message header...");
            asio::async_read_until(m_socket, m_message_buffer, '\r\n',
                [this](std::error_code ec, std::size_t bytes_transferred)
                {
                    size_t buffer_size = asio::buffer_size(m_message_buffer.data());
                    log::debug("Read {} header bytes. Current buffer size = {}", bytes_transferred, buffer_size);
                    std::stringstream ss;
                    copy_bytes_from_buffer(ss, m_message_buffer, bytes_transferred);
                    m_request.populate_headers_from_stream(ss);

                    if (ss.str().size() == 0) 
                    {
                        // Finished reading header so now we decide what we need to do
                        // Check the content-length, if its zero then ther is no body and we are done reading
                        // Now route the message and execute the appropriate handler
                        auto [success, header_value] = m_request.headers.get("content-length");
                        if ((success && header_value == "0") || !success)
                        {
                            // ROUTING
                            log::info("Executing route {} handler {}", m_request.target, "HANDLER HERE");
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

                    //  std::string line, ignore;
                    //  std::istream stream {&pThis->buff};
                    //  std::getline(stream, line, '\r');
                    //  std::getline(stream, ignore, '\n');
                    //  pThis->headers.on_read_header(line);
                    
                    //  if(line.length() == 0)
                    //  {
                    //     if(pThis->headers.content_length() == 0)
                    //     {
                    //        std::shared_ptr<std::string> str = std::make_shared<std::string>(pThis->headers.get_response());
                    //        asio::async_write(pThis->socket, boost::asio::buffer(str->c_str(), str->length()), [pThis, str](const error_code& e, std::size_t s)
                    //        {
                    //           std::cout << "done" << std::endl;
                    //        });
                    //     }
                    //     else
                    //     {
                    //        pThis->read_body(pThis);
                    //     }
                    //  }
                    //  else
                    //  {
                    //     pThis->read_next_line(pThis);
                    //  }
                }
            );
        }

        void read_request_status()
        {
            log::debug("Reading request status line...");
            asio::async_read_until(m_socket, m_message_buffer, '\r\n', 
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
                        // std::string method_str;
                        // ss >> method_str;
                        // m_request.method = method_from_string(method_str); // HttpMethod
                        // ss >> m_request.target; // String
                        // std::string version_str;
                        // ss >> version_str;
                        // m_request.version = version_from_string(version_str); // HttpVersion
                        log::debug("Request Recieved:\r\n\r\n{}\r\n", m_request.to_string());
                        // Re-prime the context to read the next part of the message
                        read_header();
                    }
                    else
                    {
                        log::error("Error reading request header: {}", ec.message());
                    }
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