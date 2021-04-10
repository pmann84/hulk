//#include "hulk/urls.hpp"
//
//#include <gtest/gtest.h>
//
//TEST(urls_tests, test_url)
//{
//    ASSERT_TRUE(true);
////    hulk::http::request request;
////    hulk::http_parser parser(request);
////
////    parser << "GET /api HTTP/1.1";
////    ASSERT_EQ(hulk::parser::status::incomplete, parser.status());
////    ASSERT_EQ(hulk::parser::read_status::request_header, parser.read_status());
////    ASSERT_EQ(hulk::HttpMethod::Get, request.method);
////    ASSERT_EQ(hulk::HttpVersion::Http11, request.version);
////    ASSERT_EQ("/api", request.target);
//}
//
//// URL slash test cases
////"/api/hello"
////"api/boo/"
////"/api//boo/hello//"