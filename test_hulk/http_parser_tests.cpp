#include "hulk/http_parser.hpp"

#include <gtest/gtest.h>

TEST(http_status_parser_tests, test_request_status_read_correctly)
{
    hulk::http::request request;
    hulk::http_parser parser(request);

    parser << "GET /api HTTP/1.1";
    ASSERT_EQ(hulk::parser::status::incomplete, parser.status());
    ASSERT_EQ(hulk::parser::read_status::request_header, parser.read_status());
    ASSERT_EQ(hulk::HttpMethod::Get, request.method);
    ASSERT_EQ(hulk::HttpVersion::Http11, request.version);
    ASSERT_EQ("/api", request.target);
}

TEST(http_status_parser_tests, test_bad_request_with_unknown_status)
{
    hulk::http::request request;
    hulk::http_parser parser(request);

    parser << "BLARG /api HTTP/1.1";
    ASSERT_EQ(hulk::parser::status::bad_request, parser.status());
}

TEST(http_status_parser_tests, test_bad_request_with_unknown_version)
{
    hulk::http::request request;
    hulk::http_parser parser(request);

    parser << "GET /api HTTP/0.0";
    ASSERT_EQ(hulk::parser::status::bad_request, parser.status());
}

TEST(http_status_parser_tests, test_request_status_with_single_query_param_read_correctly)
{
    hulk::http::request request;
    hulk::http_parser parser(request);

    parser << "GET /api?a=1 HTTP/1.1";
    ASSERT_EQ(hulk::parser::status::incomplete, parser.status());
    ASSERT_EQ(hulk::parser::read_status::request_header, parser.read_status());
    ASSERT_EQ(hulk::HttpMethod::Get, request.method);
    ASSERT_EQ(hulk::HttpVersion::Http11, request.version);
    
    ASSERT_EQ(1, request.query_parameters.size());
    auto [lookup_success, query_param_value] = request.query_parameters.get("a");
    ASSERT_EQ(true, lookup_success);
    ASSERT_EQ("1", query_param_value);
    
    ASSERT_EQ("/api", request.target);
}

TEST(http_status_parser_tests, test_request_status_with_multiple_query_param_read_correctly)
{
    hulk::http::request request;
    hulk::http_parser parser(request);

    parser << "GET /api?a=1&b=2&c=3 HTTP/1.1";
    ASSERT_EQ(hulk::parser::status::incomplete, parser.status());
    ASSERT_EQ(hulk::parser::read_status::request_header, parser.read_status());
    ASSERT_EQ(hulk::HttpMethod::Get, request.method);
    ASSERT_EQ(hulk::HttpVersion::Http11, request.version);
    
    ASSERT_EQ(3, request.query_parameters.size());
    auto [lookup_success_a, query_param_value_a] = request.query_parameters.get("a");
    ASSERT_EQ(true, lookup_success_a);
    ASSERT_EQ("1", query_param_value_a);
    
    auto [lookup_success_b, query_param_value_b] = request.query_parameters.get("b");
    ASSERT_EQ(true, lookup_success_b);
    ASSERT_EQ("2", query_param_value_b);

    auto [lookup_success_c, query_param_value_c] = request.query_parameters.get("c");
    ASSERT_EQ(true, lookup_success_c);
    ASSERT_EQ("3", query_param_value_c);

    ASSERT_EQ("/api", request.target);
}

TEST(http_status_parser_tests, test_request_status_with_multiple_query_param_badly_formed)
{
    hulk::http::request request;
    hulk::http_parser parser(request);

    parser << "GET /api?a=1b=2&c=3 HTTP/1.1";
    ASSERT_EQ(hulk::parser::status::bad_request, parser.status());
    ASSERT_EQ(hulk::parser::read_status::request_header, parser.read_status());
}

TEST(http_status_parser_tests, test_request_status_with_query_specifier_but_no_args_badly_formed)
{
    hulk::http::request request;
    hulk::http_parser parser(request);

    parser << "GET /api? HTTP/1.1";
    ASSERT_EQ(hulk::parser::status::bad_request, parser.status());
    ASSERT_EQ(hulk::parser::read_status::request_header, parser.read_status());
}