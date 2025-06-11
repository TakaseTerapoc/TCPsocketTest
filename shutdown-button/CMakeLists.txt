cmake_minimum_required(VERSION 3.10)
project(gpio_test)

set(CMAKE_CXX_STANDARD 17)

# gpiod をリンク
find_library(GPIOD_LIB gpiod REQUIRED)
include_directories(/opt/rpi/sysroot/usr/include)
link_directories(/opt/rpi/sysroot/usr/lib/aarch64-linux-gnu)

add_executable(main_exec main.cpp)
target_link_libraries(main_exec ${GPIOD_LIB})

