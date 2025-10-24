cmake_minimum_required(VERSION 3.50)
project(route_tracer CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add executable
add_executable(route_tracer
    src/main.cpp
    src/osmium_test.cpp
)

# Add GLFW subdirectory
add_subdirectory(libs/glfw)

# Add custom CMake module paths
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/libs/libosmium/cmake"
    "${CMAKE_SOURCE_DIR}/libs/protozero/cmake"
)

# Find dependencies
find_package(Osmium REQUIRED COMPONENTS io pbf)
find_package(Protozero REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
target_include_directories(route_tracer PRIVATE
    src/include/
    libs/glfw/include
    ${OSMIUM_INCLUDE_DIRS}
    ${PROTOZERO_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(route_tracer PRIVATE
    glfw
    OpenGL::GL
    ZLIB::ZLIB
    ${OSMIUM_LIBRARIES}
)