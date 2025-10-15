cmake_minimum_required(VERSION 3.5)
project(route_tracer CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(route_tracer src/main.cpp)

add_subdirectory(libs/glfw)

target_include_directories(route_tracer PRIVATE libs/glfw/include)

find_package(OpenGL REQUIRED)
target_link_libraries(route_tracer PUBLIC glfw OpenGL::GL)