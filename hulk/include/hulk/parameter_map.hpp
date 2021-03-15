#ifndef HULK_HTTP_HEADERS_H_
#define HULK_HTTP_HEADERS_H_

#include <map>
#include <string>

namespace hulk
{
    class parameter_map
    {
    public:
        parameter_map() = default;

        std::pair<bool, std::string> get(std::string key)
        {
            auto result_it = m_parameters.find(key);
            if (result_it != m_parameters.end())
            {
                return std::make_pair(true, m_parameters[key]);
            }
            return std::make_pair(false, "");
        }

        void add(std::string key, std::string value)
        {
            m_parameters[key] = value;
        }

        const std::map<std::string, std::string>& headers() const
        {
            return m_parameters;
        }

    private:
        std::map<std::string, std::string> m_parameters;
    };
}

#endif // HULK_HTTP_HEADERS_H_