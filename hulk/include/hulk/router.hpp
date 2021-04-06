#ifndef HULK_ROUTER_H_
#define HULK_ROUTER_H_

#include "response.hpp"
#include "urls.hpp"

#include <functional>
#include <map>

namespace hulk
{
    namespace http
    {
        struct request;
    }
    struct url_details;

    using route_handler_t = std::function<http::response(const http::request&)>;
    route_handler_t empty_route_handler = [](const http::request&){ return http::response::not_found(); };

    class router
    {
    public:
        router() = default;

        void add(std::string url, route_handler_t handler)
        {
            rule new_rule(url);
            m_route_map[new_rule] = handler;
        }

        std::pair<route_handler_t, url_parameters> try_get(std::string url)
        {
            // TODO: Implement < operator for rule and string then use find rather than this loop
            for (auto& [route_rule, route_handler] : m_route_map)
            {
                if (match(url, route_rule))
                {
                    log::debug("Found matching route {} for target {}", static_cast<std::string>(route_rule), url);
                    // TODO: Parse the route parameters here
                    //...
                    return std::make_pair(route_handler, );
                }
            }
            return std::make_pair(empty_route_handler, );
        }

        std::vector<rule> routes()
        {
            std::vector<rule> keys;
            std::for_each(m_route_map.begin(), m_route_map.end(), [&keys](std::pair<rule, route_handler_t> pair){ keys.push_back(pair.first); });
            return keys;
        }

    private:
        std::map<rule, route_handler_t> m_route_map;
    };
}

#endif // HULK_ROUTER_H_