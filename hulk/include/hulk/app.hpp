#ifndef HULK_APP_H_
#define HULK_APP_H_

#include "logging.hpp"

#include <cstdint>
#include <thread>

namespace hulk
{
    class app final
    {
    public:
        app() : m_server(8080), m_debug(false)
        {
            log::init(m_debug);
        }

        app& debug()
        {
            m_debug = true;
            log::set_level(spdlog::level::debug);
            return *this;
        }

        void run()
        {
            m_server.run();
        }
        
    private:
        server m_server;
        bool m_debug;
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