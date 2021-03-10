#include "connection.hpp"
#include "response.hpp"

// #include <asio.hpp>

namespace hulk
{
    class server
    {
    public:
        server(uint32_t port) 
            : m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
        { 
        }

        ~server()
        {
            // Shut down gracefully
            m_context.stop();
            if (m_context_run_thread.joinable())
            {
                m_context_run_thread.join();
            }
            log::info("Server Shutdown. Goodbye!");
        }

        void run()
        {
            // First assign some work for the context to do so it doesnt shut down immediately on startup
            log::debug("Priming context on read thread to wait for messages.");
            accept_connections();
            log::debug("Running context in separate thread.");
            m_context_run_thread = std::thread([this](){ m_context.run(); });
            log::info("Server started successfully!");
            log::info("Server listening on http://127.0.0.1:{}", m_acceptor.local_endpoint().port());

            // http::response response;
            // response.status = 404;
            // response.add_date_header();
            // log::debug(response.to_string());
            
            while(!m_context.stopped()) 
            {
                update();
            } // Run indefinitely until the context runs out of work to do
        }

        void update()
        {
            // process messages off the queue here
            if (!m_active_connection_updated)
            {
                m_active_connection_updated = true;
                log::debug("Active Connections: {}", m_active_connections.size());
            }
        }

    private:
        void accept_connections()
        {
            m_acceptor.async_accept(
                [this](std::error_code error_code, asio::ip::tcp::socket socket)
                {
                    if (!error_code)
                    {
                        // OK!
                        log::debug("Recieved new connection on http://127.0.0.1:{}", socket.remote_endpoint().port());
                        auto conn = std::make_shared<connection>(std::move(socket));
                        m_active_connections.push_back(conn); //TODO Sort this out as the connections never get disposed
                        m_active_connection_updated = false;
                        // Create a connection object to deal with reading the message - decide how 
                        // long these will live, we could store these until the message and route has 
                        // been completely processed, then use it write the response out or just create 
                        // a new connection every time - depends on how we will do the writing
                        conn->start();
                    }
                    else
                    {
                        log::error("Errored while trying to accept new connections. {}", error_code.message());
                    }
                    accept_connections();
                }
            );
        }

    private:
        asio::io_context m_context; // IO Context
        asio::ip::tcp::acceptor m_acceptor; // Acceptor that listens on endpoint (abstracts the socket)
        std::thread m_context_run_thread;
        std::vector<std::shared_ptr<connection>> m_active_connections;
        bool m_active_connection_updated = false;
        // message_queue m_message_queue;
        // std::vector<std::shared_ptr<connection>> m_connections;
        // std::vector<std::string> m_messages;
        // std::streambuf m_message_buffer; // Messages are read asynchronously so we store the message here until its ready
        // message processing queue
    };
}