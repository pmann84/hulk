#include <asio.hpp>

namespace hulk
{
    class server
    {
    public:
        server(uint32_t port) 
            : m_port(port)
            , m_endpoint(asio::ip::tcp::v4(), m_port)
            , m_acceptor(m_context, m_endpoint)
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
            hulk::log::info("Server Shutdown. Goodbye!");
        }

        void run()
        {
            // First assign some work for the context to do so it doesnt shut down immediately on startup
            // hulk::log::debug("Priming context to read thread.");
            // wait_for_message(acceptor, ctx);
            hulk::log::debug("Starting context in thread.");
            m_context_run_thread = std::thread([this](){ m_context.run(); });
            hulk::log::info("Server started successfully!");
            hulk::log::info("Server listening on 127.0.0.1:{}", m_endpoint.port());
            while(!m_context.stopped()) {} // Run indefinitely until the context runs out of work to do
        }
    private:
        uint32_t m_port;
        asio::io_context m_context; // IO Context
        asio::ip::tcp::endpoint m_endpoint; // ep(asio::ip::tcp::v4(), port);
        asio::ip::tcp::acceptor m_acceptor; //(ctx, ep); // Acceptor that listens on endpoint (abstracts the socket)
        std::thread m_context_run_thread;
    };
}