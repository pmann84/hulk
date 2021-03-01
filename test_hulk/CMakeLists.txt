cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(test_hulk)
set(SOURCES basic_tests.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(test_hulk PUBLIC Hulk)

target_link_libraries(test_hulk PUBLIC gtest_main)
target_link_libraries(test_hulk PUBLIC gmock)
target_link_libraries(test_hulk PUBLIC gmock_main)
target_link_libraries(test_hulk PUBLIC gtest)