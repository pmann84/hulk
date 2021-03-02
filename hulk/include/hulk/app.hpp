#include <cstdint>
#include <thread>

namespace hulk
{
    struct app_config
    {
        uint32_t port = 5000;
        uint32_t core_count = 1;
    };

    // Flesh out api first
    class app
    {
    public:
        app& port(uint32_t port) { m_config.port = port; }
        app& multithreaded(unsigned int core_count = std::thread::hardware_concurrency()) { m_config.core_count = core_count; } 
        
        app_config& config() { return m_config; }

        void route(std::string url) {};

        void run();

    private:
        app_config m_config;
    };
}