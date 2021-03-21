#ifndef HULK_HELPERS_H_
#define HULK_HELPERS_H_

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
}

#endif // HULK_HELPERS_H_