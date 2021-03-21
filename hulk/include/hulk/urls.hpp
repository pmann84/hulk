#ifndef HULK_URLS_H_
#define HULK_URLS_H_

#include<vector>
#include<string>

namespace hulk
{
    namespace parse
    {
        template<typename CharT>
        std::vector<std::basic_string<CharT>> split(const std::basic_string<CharT>& string_to_split, const std::basic_string<CharT>& delimiter)
        {
            std::vector<std::basic_string<CharT>> split_string;
            if (string_to_split.empty()) return split_string;
            std::size_t pos = string_to_split.find(delimiter);
            std::size_t initial_pos = 0;

            // Decompose statement
            while (pos != std::basic_string<CharT>::npos)
            {
                split_string.push_back(string_to_split.substr(initial_pos, pos - initial_pos));
                initial_pos = pos + delimiter.size();
                pos = string_to_split.find(delimiter, initial_pos);
            }

            // Add the last one
            split_string.push_back(string_to_split.substr(initial_pos, std::min(pos, string_to_split.size()) - initial_pos + 1));

            return split_string;
        }
    }

    class rule
    {
    public:
        explicit rule(std::string route) 
            : m_route(route)
        {
            
        }

        std::string route() const
        {
            return m_route;
        }

        void tokenise()
        {
            std::vector<std::string> tokens = parse::split(m_route, std::string("/"));
            return;
        }

    private:
        std::string m_route;
    };

    bool operator<(const rule& r1, const rule& r2) 
    {
        return r1.route() < r2.route();
    }



    // struct url_details
    // {
        
    // };

    // struct url_token
    // {
    //     std::string name;
    // };

    // struct url_token_parameter
    // {
    //     std::string type;
    // };

    // class rule
    // {
    // public:
    //     rule(std::string route) : m_route(route)
    //     {
    //         tokenise();
    //     }

    //     std::string route() const
    //     {
    //         return m_route;
    //     }

    // private:
    //     void tokenise()
    //     {
    //         std::vector<std::string> string_tokens = parsing_utils::split(m_route, std::string("/"));
    //         for (auto string_token : string_tokens)
    //         {
    //             // TODO: Handle multi slashes
    //             if (&string_token[0] == "<")
    //             {
    //                 string_tokens.push_back(string_token);
    //                 // Parameter token
    //             }
    //             else
    //             {
    //                 string_tokens.push_back(string_token);
    //                 // regular token
    //             }
    //         }
    //     }

    // private:
    //     std::string m_route;
    //     std::string m_view_alias;
    //     // std::vector<url_token> m_tokens;
    // };

    // bool operator<(const rule& r1, const rule& r2)
    // {
    //     return r1.route() < r2.route();
    // }
}

#endif // HULK_URLS_H_