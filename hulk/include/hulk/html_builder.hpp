#ifndef HULK_HTML_BUILDER_H_
#define HULK_HTML_BUILDER_H_

#include <sstream>
#include <vector>
#include <iostream>

namespace hulk
{
    class html_element
    {
    public:
        html_element(std::string name, std::string value) : m_name(name), m_value(value)
        {
        }

        html_element(std::string name) : html_element(name, "")
        {
        }

        html_element() : html_element("html", "")
        {
        }

        html_element& add(std::string name, std::string value)
        {
            m_children.push_back({name, value});
            return m_children.back();
        }

        html_element& add(std::string name)
        {
            return add(name, "");
        }

        std::string to_string() const
        {
            std::stringstream ss;
            ss << "<" << m_name << ">";
            if (!m_value.empty())
            {
                ss << m_value;
            }
            for (auto child : m_children)
            {
                ss << child.to_string();
            }
            ss << "</" << m_name << ">";
            return ss.str();
        }
    private:
        std::string m_name;
        std::string m_value;
        std::vector<html_element> m_children;
    };

//    class html_builder
//    {
//    public:
//        html_builder()
//        {
//            m_root.m_name = "html";
//        }
//
//        html_element& add_element(std::string name, std::string value = "", html_element* e = nullptr)
//        {
//            html_element new_element;
//            new_element.m_name = name;
//            if (!value.empty())
//            {
//                new_element.value = value;
//            }
//
//            if (e)
//            {
//                e->m_children.push_back(new_element);
//            }
//            else
//            {
//                m_root.m_children.push_back(new_element);
//            }
//            return new_element;
//        }
//
//        std::string build()
//        {
//            std::stringstream ss;
//            ss << m_root.to_string();
//            return ss.str();
//        }
//
//    private:
//        html_element m_root;
//    };
}

#endif //HULK_HTML_BUILDER_H_

