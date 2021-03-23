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
        app() : m_debug(false), m_port(8080)
        {
            log::init(m_debug);
        }

        app& debug()
        {
            m_debug = true;
            log::set_level(spdlog::level::debug);
            return *this;
        }

        app& port(uint32_t port)
        {
            m_port = port;
            return *this;
        }

        void route(std::string url, route_handler_t handler)
        {
            log::info("Registering route: {}", url);
            m_router.add(url, handler);
        }

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



    // struct app_config
    // {
    //     uint32_t port = 5000;
    //     uint32_t core_count = 1;
    // };

    // // Flesh out api first
    // class app
    // {
    // public:
    //     app& port(uint32_t port) { m_config.port = port; }
    //     app& multithreaded(unsigned int core_count = std::thread::hardware_concurrency()) { m_config.core_count = core_count; } 
        
    //     app_config& config() { return m_config; }

    //     void route(std::string url) {};

    //     void run();

    // private:
    //     app_config m_config;
    // };
}

#endif // HULK_APP_H_