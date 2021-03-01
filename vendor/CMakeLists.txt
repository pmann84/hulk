add_subdirectory(spdlog)
# TODO: Make this condition with a build tests flag
add_subdirectory(googletest)

# Add asio include directory for parent scope
set(ASIO_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio/include PARENT_SCOPE)