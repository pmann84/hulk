#ifndef HULK_HTTP_BODY_H_
#define HULK_HTTP_BODY_H_

#include <sstream>

namespace hulk
{
    class http_body
    {
    public:
        http_body() = default;

        void data_type(std::string data_type)
        {
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

        uint64_t content_length() const
        {
            return m_data.size();
        }

        std::string content_type() const
        {
            return m_data_type;
        }

        http_body& operator<<(std::string s)
        {
            m_data.append(s);
            return *this;
        } 

    private:
        std::string m_data;
        std::string m_data_type;
    };
}

#endif // HULK_HTTP_BODY_H_