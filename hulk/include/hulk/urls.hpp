#ifndef HULK_URLS_H_
#define HULK_URLS_H_

#include "string_helpers.hpp"
#include "logging.hpp"

#include<vector>
#include<string>
#include<sstream>
#include<regex>

namespace hulk
{
    namespace parse
    {
        std::vector<std::string> tokenise_url(const std::string& url)
        {
            std::vector<std::string> tokens = strings::split(url, std::string("/"));
            // Basically we check to see if there are any odd slashes, i.e.
            // double /'s, trailing or preceeding /'s then we just ignore them
            // as they make no difference
            tokens.erase(std::remove_if(
                    tokens.begin(),
                    tokens.end(),
                    [](std::string& token)
                    {
                        return token.empty();
                    }
            ), tokens.end());
            return tokens;
        }

        std::string strip_parameter_token(const std::string& token)
        {
            auto trimmed_token = strings::trim_left(token, '<');
            return strings::trim_right(trimmed_token, '>');
        }

        bool is_parameter_token(const std::string& token)
        {
            return strings::has_special_char(token);
        }

        bool validate_parameter_token(const std::string& token)
        {
            // Parameter should have the form <type>
            if (strings::starts_with(token, std::string("<")) && strings::ends_with(token, std::string(">")))
            {
                auto trimmed_token = strip_parameter_token(token);
                return ParamTypeMap.find(trimmed_token) != ParamTypeMap.end();
            }
            return false;
        }
    }

    struct url_token
    {
        std::string name;

        bool is_parameter() const
        {
            return parse::is_parameter_token(name);
        }

        param_type get_type() const
        {
            auto stripped_name = parse::strip_parameter_token(name);
            return ParamTypeMap[stripped_name];
        }
    };

    class rule
    {
    public:
        explicit rule(const std::string& route)
                : m_route(route)
        {
            deconstruct();
            validate();
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

        void validate()
        {
            for (auto token : m_tokens)
            {
                if (token.is_parameter() && !parse::validate_parameter_token(token.name))
                {
                    std::stringstream err_ss;
                    err_ss << "Attempt to register invalid url ";
                    err_ss << m_route << ": Invalid parameter token " << token.name;
                    log::error(err_ss.str());
                    throw std::runtime_error(err_ss.str()); // TODO: Handle this not by throwing but exiting gracefully
                }
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
            log::debug("Attempting to match route parameter {} to {}", rule_token.name, target_token);
            if (rule_token.is_parameter())
            {
                // Need to parse the params
//                log::debug("Parameter detected...continue...");
            }
            else
            {
                // Just check for equality here
                if (rule_token.name != target_token) return false;
//                log::debug("Token matched...continue...");
            }
        }
        log::debug("Matched target url [{}] to route [{}]", target, r.route());
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