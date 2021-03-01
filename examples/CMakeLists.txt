add_executable(example_app example_app.cpp)

target_include_directories(example_app PUBLIC ${ASIO_INCLUDE_DIRECTORIES})