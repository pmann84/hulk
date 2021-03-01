#include <spdlog/spdlog.h>

namespace hulk
{
    namespace log
    {
        void init()
        {
            spdlog::set_pattern("%^[%d/%m/%Y %T.%e][%l][thread %t] %v%$");
            spdlog::set_level(spdlog::level::debug); 
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
