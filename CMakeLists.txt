cmake_minimum_required(VERSION 3.50)
project(route_tracer CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(BUILD_EXAMPLES OFF)

# Add executable
add_executable(route_tracer
    src/main.cpp
    src/map_data.cpp
    src/a_star.cpp
    src/renderer.cpp
    src/windower.cpp
    libs/glad/src/glad.c
)

# Add custom CMake module paths

find_package(ZLIB REQUIRED)
find_package(EXPAT REQUIRED)
find_package(BZip2 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
target_include_directories(route_tracer PRIVATE
    libs/libosmium/include
    libs/protozero/include
    libs/glad/include
)



# Link libraries
target_link_libraries(route_tracer PRIVATE
    glfw
    OpenGL::GL
    ZLIB::ZLIB
    BZip2::BZip2
    EXPAT::EXPAT
)