# HULK

A C++ micro web framework with an emphasis on a usable interface.

## Overview
Hulk is a header only C++ micro web framework written using the asio framework.

## Features

## Development
Since this is a header only library, it doesn't require building, but there are tests and examples you can build and run. Currently Hulk supports the CMake build system and tests are written using the googletest framework. To build the tests do an out of source build by running
```
mkdir out
cd out
cmake ..
cmake --build . --config Debug
```

## Future Development
Below is a list of issues that need addressing - these will eventually move to github issues tracking
- Middleware Response caching
- Support for query parameters e.g. localhost:8080/hello?p1=5&p2=world etc etc
- Investigate usage of classes for routes
- Implement url namespaces i.e. providing a prefix to a set of api end points e.g. api/hello and api/world so you dont have to specify the prefix "api"