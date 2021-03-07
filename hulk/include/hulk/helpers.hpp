#include <string>

namespace hulk
{
    namespace strings
    {
        template<typename CharT>
        std::basic_string<CharT> trim_right(const std::basic_string<CharT>& string_to_trim, const CharT delimiter)
        {
            if (string_to_trim.empty()) return std::basic_string<CharT>();
            typename std::basic_string<CharT>::const_iterator p = string_to_trim.cend() - 1;
            while (*p == delimiter) --p;
            ++p; // Increment to get correct end point
            return p == string_to_trim.cbegin() ? std::basic_string<CharT>() : std::basic_string<CharT>(string_to_trim.begin(), p);
        }
    }
}