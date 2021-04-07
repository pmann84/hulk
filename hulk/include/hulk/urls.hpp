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

        std::vector<std::string> split_parameter_token(const std::string& token)
        {
            std::string stripped_token = strip_parameter_token(token);
            std::vector<std::string> param_split = strings::split(stripped_token, std::string(":"));
            return param_split;
        }

        std::pair<std::string, std::string> parse_parameter_pair(const std::string& token_str)
        {
            std::vector<std::string> param_split = split_parameter_token(token_str);
            return {param_split[0], param_split[1]};
        }

        bool is_parameter_token(const std::string& token)
        {
            return strings::has_special_char(token);
        }

        bool validate_parameter_token(const std::string& token)
        {
            // Parameter should have the form <type:name>
            if (strings::starts_with(token, std::string("<")) && strings::ends_with(token, std::string(">")))
            {
                log::debug("{} is parameter type", token);
                if (split_parameter_token(token).size() != 2)
                {
                    return false;
                }
                auto [param_type, param_name] = parse_parameter_pair(token);
                return ParamTypeMap.find(param_type) != ParamTypeMap.end();
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

        std::string get_name() const
        {
            if (parse::validate_parameter_token(name))
            {
                auto[param_type, param_name] = parse::parse_parameter_pair(name);
                return param_name;
            }
            return "";
        }

        param_type get_type() const
        {
            if (parse::validate_parameter_token(name))
            {
                auto[param_type, param_name] = parse::parse_parameter_pair(name);
                return ParamTypeMap[param_type];
            }
            return param_type::none_type;
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
                log::debug("Validating parameter {}", token.name);
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

    template <typename ConvertFuncT>
    bool is_convertible(ConvertFuncT func)
    {
        try
        {
            func();
            return true;
        }
        catch (std::exception& e)
        {
            // std::invalid_argument if no conversion could be performed
            // std::out_of_range if the converted value would fall out of the range of the
            // result type or if the underlying function (std::strtol or std::strtoll)
            // sets errno to ERANGE.
            log::debug("Target value isn't convertible to parameter type.");
            return false;
        }
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
                // Check that it can be converted to the specified type
                switch (rule_token.get_type())
                {
                    case param_type::int_type:
                    {
                        return is_convertible([&target_token](){ std::stoi(target_token); });
                    }
                    case param_type::float_type:
                    {
                        return is_convertible([&target_token](){ std::stof(target_token); });
                    }
                    case param_type::double_type:
                    {
                        return is_convertible([&target_token](){ std::stod(target_token); });
                    }
                    case param_type::string_type:
                    {
                        return true;
                    }
                    default:
                        return false;
                }
            }
            else
            {
                // Just check for equality here
                if (rule_token.name != target_token) return false;
            }
        }
        log::debug("Matched target url [{}] to route [{}]", target, r.route());
        return true;
    }

    url_parameters parse_url_params(const std::string& target, const rule& r)
    {
        url_parameters params;
        std::vector<std::string> target_tokens = parse::tokenise_url(target);
        for (size_t i = 0; i < r.tokens().size(); ++i)
        {
            const url_token rule_token = r.tokens()[i];
            const std::string target_token = target_tokens[i];
            log::debug("Attempting to parse parameter {} to {}", rule_token.name, target_token);
            if (rule_token.is_parameter())
            {
                url_parameter param_value;
                switch (rule_token.get_type())
                {
                    case param_type::int_type:
                    {
                        log::debug("Parsing int parameter...");
                        param_value = static_cast<uint64_t>(std::stoi(target_token));
                        break;
                    }
                    case param_type::float_type:
                    {
                        log::debug("Parsing float parameter...");
                        param_value = std::stof(target_token);
                        break;
                    }
                    case param_type::double_type:
                    {
                        log::debug("Parsing double parameter...");
                        param_value = std::stod(target_token);
                        break;
                    }
                    case param_type::string_type:
                    {
                        log::debug("Parsing string parameter...");
                        param_value = target_token;
                        break;
                    }
                }
                auto param_name = rule_token.get_name();
                params[param_name] = param_value;
            }
        }
        return params;
    }
}

#endif // HULK_URLS_H_