#ifndef HULK_LOGGING_H_
#define HULK_LOGGING_H_

#include <spdlog/spdlog.h>

#include <string>

namespace hulk
{
    namespace log
    {
        constexpr char debug_log_pattern[] = "%^[%d/%m/%Y %T.%e][%l][thread %t] %v%$";
        constexpr char log_pattern[] = "%^[%d/%m/%Y %T.%e][%l] %v%$";

        void set_level(spdlog::level::level_enum level)
        {
            spdlog::set_pattern(level == spdlog::level::debug ? debug_log_pattern : log_pattern);
            spdlog::set_level(level); 
        }

        void init(bool debug = false)
        {
            set_level(debug ? spdlog::level::debug : spdlog::level::info);
        }

        template<typename FormatString, typename... Args>
        inline void error(const FormatString &fmt, Args&&...args)
        {
            spdlog::error(fmt, args...);
        }

        template<typename FormatString, typename... Args>
        inline void warn(const FormatString &fmt, Args&&...args)
        {
            spdlog::warn(fmt, args...);
        }

        template<typename FormatString, typename... Args>
        inline void critical(const FormatString &fmt, Args&&...args)
        {
            spdlog::critical(fmt, args...);
        }

        template<typename FormatString, typename... Args>
        inline void info(const FormatString &fmt, Args&&...args)
        {
            spdlog::info(fmt, args...);
        }

        template<typename FormatString, typename... Args>
        inline void debug(const FormatString &fmt, Args&&...args)
        {
            spdlog::debug(fmt, args...);
        }
    }
} 

#endif // HULK_LOGGING_H_