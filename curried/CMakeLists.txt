cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17 -Wall -Wextra -pedantic -Werror -Wno-gnu-zero-variadic-macro-arguments)

project(Tutorial)

add_executable(Tutorial src/main.cpp src/curried.h)
