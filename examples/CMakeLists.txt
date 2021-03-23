cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(simple_api simple_api.cpp)
target_link_libraries(simple_api PUBLIC Hulk)

add_executable(json_rest_api json_rest_api.cpp)
target_link_libraries(json_rest_api PUBLIC Hulk)

add_executable(parameterised_urls parameterised_urls.cpp)
target_link_libraries(parameterised_urls PUBLIC Hulk)
