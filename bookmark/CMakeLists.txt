cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17 -Wall -Wextra -pedantic -Werror -Wno-gnu-zero-variadic-macro-arguments)

# set the project name
project(Tutorial)

# add the executable
add_executable(Tutorial
    src/expected.h
    src/filter.h
    src/mtry.h
    src/join.h
    src/json.hpp
    src/main.cpp
    src/transform.h
    src/service.cpp
    src/service.h
    src/session.h
    src/sink.h
    src/values.h
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Tutorial PRIVATE Threads::Threads)
