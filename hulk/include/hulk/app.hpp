#ifndef HULK_APP_H_
#define HULK_APP_H_

#include "logging.hpp"
#include "server.hpp"
#include "router.hpp"

#include <cstdint>
#include <thread>
#include <memory>

namespace hulk
{
    class app final
    {
    public:
        /// \brief Application constructor
        app() : m_debug(false), m_port(8080)
        {
            log::init(m_debug);
        }

        /// \brief Sets the debug flag for the application. Enables debug logging.
        /// \return Reference to the application.
        app& debug()
        {
            m_debug = true;
            log::set_level(spdlog::level::debug);
            return *this;
        }

        /// \brief Sets the port that the server will listen on.
        /// \param port Port the server will listen on. Default: 8080
        /// \return Reference to the application.
        app& port(uint32_t port)
        {
            m_port = port;
            return *this;
        }

        /// \brief Registers a new route with the application.
        ///
        /// This accepts URLs that are delimited by /'s. Consecutive /'s are collapsed, and can accept parameters in the form \<type\>, where type is one of int, float, double or string.
        /// \param url The url path to associate with the handler
        /// \param handler A handler to execute for the given url, this must be a function that returns a hulk::http::response, and accepts a hulk::http::request and any other parameters present in the URL specified.
        void route(std::string url, route_handler_t handler)
        {
            log::info("Registering route: {}", url);
            m_router.add(url, handler);
        }

        /// \brief Runs the application.
        void run()
        {
            m_server = std::make_unique<server>(m_port, m_router);
            m_server->run();
        }
        
    private:
        bool m_debug;
        uint32_t m_port;
        router m_router;
        std::unique_ptr<server> m_server;
    };
}

#endif // HULK_APP_H_