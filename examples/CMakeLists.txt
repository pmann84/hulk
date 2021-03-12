set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(simple_api simple_api.cpp)

target_link_libraries(simple_api PUBLIC Hulk)