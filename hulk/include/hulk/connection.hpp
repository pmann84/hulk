#ifndef HULK_CONNECTION_H_
#define HULK_CONNECTION_H_

#include "logging.hpp"

#include <asio.hpp>

namespace hulk
{
    class connection
    {
    public:
        connection(asio::ip::tcp::socket socket) : m_socket(std::move(socket))
        {
           log::debug("Creating new connection to handle incoming message.");
        }

        void start()
        {
            read();
        }

    private:
        // ASYNC
        void read()
        {
            // log::debug("ENTERED READ.");
            asio::async_read(m_socket, m_message_buffer,
                [this](std::error_code e, std::size_t length)
                {
                    if (!e)
                    {
                        size_t buffer_size = asio::buffer_size(m_message_buffer.data());
                        log::debug("Read {} bytes. Current buffer size = {}", length, buffer_size);
                        std::stringstream ss;
                        auto const_buffer = m_message_buffer.data();
                        std::copy(
                            asio::buffers_begin(const_buffer),
                            asio::buffers_begin(const_buffer) + buffer_size,
                            std::ostream_iterator<char>(ss)
                        );
                        hulk::log::info("Message recieved: {}", ss.str());
                        m_message_buffer.consume(buffer_size);
                        read();
                    }
                    else
                    {
                        log::error("Error reading message: {}", e.message());
                        m_socket.close();
                    }
                }
            );
        }

    private:
        asio::ip::tcp::socket m_socket;
        asio::streambuf m_message_buffer; // Messages are read asynchronously so we store the message here until its ready
    };
}

#endif // HULK_CONNECTION_H_