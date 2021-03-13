cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJECT_NAME "Hulk")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} 
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include 
        ${SPDLOG_INCLUDE_DIRECTORIES}
        ${ASIO_INCLUDE_DIRECTORIES}
        ${JSON_INCLUDE_DIRECTORIES}
)

target_sources(
    ${PROJECT_NAME} 
    INTERFACE 
        "include/hulk/hulk.hpp"
)