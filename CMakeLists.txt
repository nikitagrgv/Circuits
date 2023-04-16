cmake_minimum_required(VERSION 3.24)
project(circuits)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_directories(lib)

set(SFML_STATIC_LIBRARIES TRUE)

add_definitions(-DSFML_STATIC)

find_package(SFML COMPONENTS graphics audio network window system REQUIRED)

add_executable(circuits src/main.cpp src/Node.h src/Signal.h src/Nodes.h src/Graph.h src/Graph.cpp)

target_link_libraries(circuits sfml-graphics sfml-audio sfml-system sfml-window)
