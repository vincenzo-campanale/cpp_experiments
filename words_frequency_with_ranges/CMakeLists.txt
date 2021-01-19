cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17)

# set the project name
project(Tutorial)

# add the executable
add_executable(Tutorial src/main.cpp)

include_directories(third_party/range-v3/include/)
