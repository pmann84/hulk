#include "response.hpp"
#include "connection_manager.hpp"
#include "router.hpp"

namespace hulk
{
    class server
    {
    public:
        server(uint32_t port, router& router) 
            : m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
            , m_router(router)
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
            std::stringstream registered_ep_ss;
            registered_ep_ss << "Registered Endpoints: " << std::endl;
            for (auto route : m_router.routes())
            {
                registered_ep_ss << route.route() << std::endl;
            }
            log::info(registered_ep_ss.str());
            while(!m_context.stopped()) 
            {
                update();
            } // Run indefinitely until the context runs out of work to do
        }

        void update()
        {
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
                        log::info("Recieved new connection on http://127.0.0.1:{}", socket.remote_endpoint().port());
                        auto conn = std::make_shared<connection>(std::move(socket), m_active_connections, m_router);
                        m_active_connections.start(conn);
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
        connection_manager m_active_connections;
        router& m_router;
    };
}