// #include <asio.hpp>
// #include <SDKDDKVer.h>

#include <hulk/logging.hpp>

int main()
{
    hulk::log::init();
    hulk::log::error("Some error message with arg: {}", 1);
    hulk::log::warn("Easy padding in numbers like {:08d}", 12);
    hulk::log::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    hulk::log::info("Support for floats {:03.2f}", 1.23456);
    hulk::log::debug("This message should be displayed..");   
    return 0;
}