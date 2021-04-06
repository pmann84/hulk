#ifndef HULK_HTML_BUILDER_H_
#define HULK_HTML_BUILDER_H_

#include <sstream>

namespace hulk
{
    class html_builder
    {
    public:
        html_builder() = default;

        html_builder& add_element(std::string&& element)
        {

        }

    private:
        std::stringstream m_html_ss;
    };
}

#endif //HULK_HTML_BUILDER_H_
