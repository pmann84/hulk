#ifndef HULK_URLS_H_
#define HULK_URLS_H_

#include<vector>
#include<string>

namespace hulk
{
    namespace parse
    {
        template<typename CharT>
        std::vector<std::basic_string<CharT>>
        split(const std::basic_string<CharT>& string_to_split, const std::basic_string<CharT>& delimiter)
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
            split_string.push_back(
                    string_to_split.substr(initial_pos, std::min(pos, string_to_split.size()) - initial_pos + 1));

            return split_string;
        }

        std::vector<std::string> tokenise_url(const std::string& url)
        {
            std::vector<std::string> tokens = parse::split(url, std::string("/"));
            // Basically we check to see if there are any odd slashes, i.e.
            // double /'s, trailing or preceeding /'s then we just ignore them
            // as they make no difference
            tokens.erase(std::remove_if(
                    tokens.begin(),
                    tokens.end(),
                    [](std::string token)
                    {
                        return token.empty();
                    }
            ), tokens.end());
            return tokens;
        }
    }

    struct url_token
    {
        std::string name;

        bool is_parameter() const
        {
            return name[0] == '<';
        }
    };

    class rule
    {
    public:
        explicit rule(const std::string& route)
                : m_route(route)
        {
            deconstruct();
        }

        std::string route() const
        {
            return m_route;
        }

        std::vector<url_token> tokens() const
        {
            return m_tokens;
        }

        explicit operator std::string() const
        {
            std::stringstream out;
            for (auto token : m_tokens)
            {
                out << "/" << token.name;
            }
            return out.str();
        }

    private:
        void deconstruct()
        {
            std::vector<std::string> tokens = parse::tokenise_url(m_route);

            for (auto& token : tokens)
            {
                url_token new_token{token};
                m_tokens.push_back(new_token);
                std::stringstream log_ss;
                log_ss << "Parsing token: " << new_token.name;
                if (new_token.is_parameter()) log_ss << " (Parameter)";
                log::debug(log_ss.str());
            }
        }

    private:
        std::string m_route;
        std::vector<url_token> m_tokens;
    };

    bool operator<(const rule& r1, const rule& r2)
    {
        return r1.route() < r2.route();
    }

    bool match(const std::string& target, const rule& r)
    {
        std::vector<std::string> target_tokens = parse::tokenise_url(target);

        if (target_tokens.size() != r.tokens().size()) return false; // Doesnt match if number of tokens is different!

        for (size_t i = 0; i < r.tokens().size(); ++i)
        {
            const url_token rule_token = r.tokens()[i];
            const std::string target_token = target_tokens[i];
            if (rule_token.is_parameter())
            {
                // Need to parse the params
                log::debug("Attempting to match route parameter {} to {}", rule_token.name, target_token);
            }
            else
            {
                // Just check for equality here
                if (rule_token.name != target_token) return false;
            }
        }
        return true;
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