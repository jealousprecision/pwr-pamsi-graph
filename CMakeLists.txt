cmake_minimum_required(VERSION 3.05)
set(CMAKE_CXX_STANDARD 17)

project(pamsi-graph VERSION 1.0)

add_executable(graph
    ./src/main.cpp)

set(CMAKE_CXX_FLAGS "-Wall")

target_include_directories(graph PUBLIC src/)
