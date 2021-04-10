#include "hulk/html_builder.hpp"

#include <gtest/gtest.h>

TEST(html_builder_tests, test_empty_root)
{
    hulk::html_element doc;
    ASSERT_EQ("<html></html>", doc.to_string());
}

TEST(html_builder_tests, test_single_element)
{
    hulk::html_element doc;
    doc.add("p", "Hello, World!");
    ASSERT_EQ("<html><p>Hello, World!</p></html>", doc.to_string());
}

TEST(html_builder_tests, test_more_than_one_element)
{
    hulk::html_element doc;
    doc.add("p", "Hello, World!");
    doc.add("p", "Hello again...");
    ASSERT_EQ("<html><p>Hello, World!</p><p>Hello again...</p></html>", doc.to_string());
}

TEST(html_builder_tests, test_nested_elements)
{
    hulk::html_element doc;
    auto& ul = doc.add("ul");
    for (int i = 0; i < 3; i++)
    {
        std::stringstream ss;
        ss << "list item " << i;
        auto& li = ul.add("li");
        li.add("p", ss.str());
    }
    ASSERT_EQ("<html><ul><li><p>list item 0</p></li><li><p>list item 1</p></li><li><p>list item 2</p></li></ul></html>", doc.to_string());
}