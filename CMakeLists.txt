cmake_minimum_required(VERSION 3.5)
project(route_tracer CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(route_tracer src/main.cpp)

target_link_libraries(route_tracer -lGL -lglfw)