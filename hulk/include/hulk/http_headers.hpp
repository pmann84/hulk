#include <map>
#include <string>

namespace hulk
{
    class http_headers
    {
    public:
        http_headers() = default;

        std::pair<bool, std::string> get(std::string key)
        {
            auto result_it = m_headers.find(key);
            if (result_it != m_headers.end())
            {
                return std::make_pair(true, m_headers[key]);
            }
            return std::make_pair(false, "");
        }

        void add(std::string key, std::string value)
        {
            m_headers[key] = value;
        }

        const std::map<std::string, std::string>& headers() const
        {
            return m_headers;
        }

    private:
        std::map<std::string, std::string> m_headers;
    };
}