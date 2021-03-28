# HULK

A C++ micro web framework with an emphasis on a usable interface.

## Overview
Hulk is a header only C++ micro web framework written using the asio framework. Why "hulk"? No particular reason...

## How to integrate with your app using CMake
Simply add the following line to your CMakeLists.txt
```
target_link_libraries(myproject PUBLIC Hulk)
```
then just include the main header
```
#include "hulk/hulk.hpp"
```

## Example
See the examples project for more, but a simple example might look like this
```c++
#include "hulk/hulk.hpp"

int main()
{
    hulk::app app;
    app.port(5000);

    // Register endpoint
    app.route("/api", 
        [](const hulk::http::request&)
        {
            // Do some stuff here... //
            return hulk::http::response::ok("<html><h1>Hello, World!</h1></html>");
        }
    );

    app.run();
}
```

## Development
Since this is a header only library, it doesn't require building, but there are tests and examples you can build and run. Currently Hulk supports the CMake build system and tests are written using the googletest framework. To build the tests do an out of source build by running
```
mkdir out
cd out
cmake ..
cmake --build . --config Debug
```

## Benchmarking
Get setup with this benchmark that runs against a variety of different frameworks
https://github.com/TechEmpower/FrameworkBenchmarks

## Current Development
Currently this is a very miminally featured web framework, that has very little url parsing support. Currently it only supports the addition of basic non parameterised url tokens
- Proper routing of urls - passing of route parameters into handlers
- Allow duplicate query parameters 
- Allow query parameters delimited by semicolon `;` as well as `&`
- Tighten up validation of http requests when parsing them
- Make logging more minimal

## Future Development
Below is a list of issues that need addressing - these will eventually move to github issues tracking
- Implement multithreading - basically implement a pool of contexts that can process requests in parallel - can only process requests from different servers in parallel
- Database layer - simple querying, not necessarily
- Middleware Response caching
- Investigate usage of classes for routes
- Implement url namespaces i.e. providing a prefix to a set of api end points e.g. api/hello and api/world so you dont have to specify the prefix "api"
- Support for websockets 
- More Unit test coverage
- Benchmarking
