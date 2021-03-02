set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(example_app example_app.cpp)

target_link_libraries(example_app PUBLIC Hulk)

target_include_directories(example_app
    PUBLIC 
        ${ASIO_INCLUDE_DIRECTORIES}
)