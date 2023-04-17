cmake_minimum_required(VERSION 3.24)
project(circuits)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_directories(lib)

set(SFML_STATIC_LIBRARIES TRUE)

add_definitions(-DSFML_STATIC)

find_package(SFML COMPONENTS graphics window system REQUIRED)

add_executable(circuits src/main.cpp src/Node.h src/Signal.h src/Nodes.h src/Graph.h src/Graph.cpp)

target_link_libraries(circuits sfml-graphics sfml-system sfml-window)

set_target_properties(circuits
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
        )