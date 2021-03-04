#include <asio.hpp>

namespace hulk
{
    class server
    {
    public:
        server(uint32_t port) : m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
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
            log::info("Server listening on http://127.0.0.1:{}",
                            m_acceptor.local_endpoint().port());
            while(!m_context.stopped()) 
            {
                update();
            } // Run indefinitely until the context runs out of work to do
        }

        void update()
        {
            // process messages off the queue here
        }

    private:
        // ASYNC
        void accept_connections()
        {
            m_acceptor.async_accept(
                [this](std::error_code error_code, asio::ip::tcp::socket socket)
                {
                    if (!error_code)
                    {
                        // OK!
                        log::debug("Recieved new message on http://127.0.0.1:{}", socket.remote_endpoint().port());
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
        asio::ip::tcp::acceptor m_acceptor; //(ctx, ep); // Acceptor that listens on endpoint (abstracts the socket)
        std::thread m_context_run_thread;
        // message processing queue
    };
}