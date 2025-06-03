cmake_minimum_required(VERSION 3.12)
project(apple1_terminal C CXX ASM)
set(CMAKE_CXX_STANDARD 14)

set(CIRCLE_HOME /path/to/circle)  # <-- Anpassen!
include(${CIRCLE_HOME}/cmake/toolchain-gcc.cmake)
include(${CIRCLE_HOME}/cmake/circle.cmake)

add_circle_executable(kernel
    src/main.cpp
    src/VT100Console.cpp
)
