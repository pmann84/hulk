#ifndef HULK_ROUTER_H_
#define HULK_ROUTER_H_

#include "response.hpp"

#include <functional>
#include <map>

namespace hulk
{
    namespace http
    {
        struct request;
    }

    using route_handler_t = std::function<http::response(const http::request&)>;

    class router
    {
    public:
        router() = default;

        void add(std::string url, route_handler_t handler)
        {
            m_route_map[url] = handler;
        }

        route_handler_t try_get(std::string url)
        {
            if (m_route_map.find(url) != m_route_map.end())
            {
                return m_route_map[url];
            }
            return [](const http::request&){ return http::response::not_found(); };
        }

    private:
        std::map<std::string, route_handler_t> m_route_map;
    };
}

#endif // HULK_ROUTER_H_