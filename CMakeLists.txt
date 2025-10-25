cmake_minimum_required(VERSION 3.50)
project(route_tracer CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(BUILD_EXAMPLES OFF)

# Add executable
add_executable(route_tracer
    src/main.cpp
    src/osmium_test.cpp
)

# Add GLFW subdirectory
add_subdirectory(libs/glfw)

# Add custom CMake module paths

find_package(ZLIB REQUIRED)
find_package(EXPAT REQUIRED)
find_package(BZip2 REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
target_include_directories(route_tracer PRIVATE
    src/include/
    libs/glfw/include
    libs/libosmium/include
    libs/protozero/include
)

# Link libraries
target_link_libraries(route_tracer PRIVATE
    glfw
    OpenGL::GL
    ZLIB::ZLIB
    BZip2::BZip2
    EXPAT::EXPAT
)