cmake_minimum_required(VERSION 3.24)
project(circuits)

set(CMAKE_CXX_STANDARD 17)

add_executable(circuits main.cpp Node.h Signal.h Nodes.h Graph.h Graph.cpp)
