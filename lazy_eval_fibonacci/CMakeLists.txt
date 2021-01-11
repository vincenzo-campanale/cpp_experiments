cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17 -Wall -Wextra -pedantic -Werror -Wno-gnu-zero-variadic-macro-arguments)

# set the project name
project(Tutorial)

add_subdirectory(third_party/benchmark)
add_subdirectory(src/lib)

add_executable(Main src/main.cpp)
target_link_libraries(Main PRIVATE Lib)

add_executable(Benchmark src/benchmark.cpp)
target_link_libraries(Benchmark benchmark::benchmark Lib)
