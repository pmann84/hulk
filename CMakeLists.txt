cmake_minimum_required(VERSION 3.10)

project(
    webcpp
    VERSION 0.0.1
    DESCRIPTION ""
    LANGUAGES CXX
)

# Disable the ZERO_CHECK project generation
set(CMAKE_SUPPRESS_REGENERATION true)

# Third party libs
add_subdirectory(vendor)

# Hulk Library
add_subdirectory(hulk)

# Examples
add_subdirectory(examples)

# Tests
add_subdirectory(test_hulk)