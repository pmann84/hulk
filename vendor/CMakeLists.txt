# TODO: Make this condition with a build tests flag
add_subdirectory(googletest)

# json directory
set(JSON_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/json/single_include PARENT_SCOPE)

# Add header only spdlog directory
set(SPDLOG_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include PARENT_SCOPE)

# Add asio include directory for parent scope
set(ASIO_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio/include PARENT_SCOPE)