#ifndef HULK_HTTP_BODY_H_
#define HULK_HTTP_BODY_H_

#include <sstream>

namespace hulk
{
    class http_body
    {
    public:
        http_body() = default;

        void add_body_data(std::string data, std::string data_type)
        {
            m_data.append(data);
            m_data_type = data_type;
        }

        // TODO: Add json data
        // TODO: Add html data
        // TODO: Add data from file
        bool empty() const
        {
            return m_data.empty();
        }
        
        std::string data() const
        {
            return m_data;
        }

        std::string content_length() const
        {
            std::stringstream ss;
            ss << "Content-Length: " << m_data.size() << "\r\n";
            return ss.str();
        }

        std::string content_type() const
        {
            std::stringstream ss;
            ss << "Content-Type: " << m_data_type << "\r\n";
            return ss.str();
        }

    private:
        std::string m_data;
        std::string m_data_type;
    };
}

#endif // HULK_HTTP_BODY_H_