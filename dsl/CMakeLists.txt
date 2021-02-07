cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17 -Wall -Wextra -pedantic -Werror -Wno-gnu-zero-variadic-macro-arguments)

# set the project name
project(Tutorial)

# add the executable
add_executable(Tutorial
    src/dsl.h
    src/main.cpp
    src/person.h
    src/vehicle.h
)

