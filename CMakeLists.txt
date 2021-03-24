cmake_minimum_required(VERSION 3.17)

project(
    hulk
    VERSION 0.0.1
    DESCRIPTION ""
    LANGUAGES CXX
)

option(BUILD_TESTS "Build the tests" ON)
option(BUILD_DOCS "Build the docs" ON)

# Who doesnt like to know the compilation commands...
set(CMAKE_VERBOSE_MAKEFILE ON)

# Disable the ZERO_CHECK project generation
set(CMAKE_SUPPRESS_REGENERATION true)

# Since googletest uses these flags we should too.
SET(CMAKE_CXX_FLAGS_DEBUG "/MTd")
SET(CMAKE_CXX_FLAGS_RELEASE "/MT")

# Third party libs
add_subdirectory(vendor)

# Hulk Library
add_subdirectory(hulk)

# Examples
add_subdirectory(examples)

# Tests
if(BUILD_TESTS)
    add_subdirectory(test_hulk)
endif(BUILD_TESTS)

# Docs
if(BUILD_DOCS)
    set(CMAKE_DOXYGEN_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen)
    add_subdirectory(doc)
endif(BUILD_DOCS)
