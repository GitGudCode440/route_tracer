cmake_minimum_required(VERSION 3.50)
project(route_tracer CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(BUILD_EXAMPLES OFF)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)

file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Add executable
add_executable(route_tracer
   ${SRC_FILES}
   ${IMGUI_DIR}/imgui.cpp
   ${IMGUI_DIR}/imgui_demo.cpp
   ${IMGUI_DIR}/imgui_draw.cpp 
   ${IMGUI_DIR}/imgui_tables.cpp 
   ${IMGUI_DIR}/imgui_widgets.cpp 
   ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
   ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
   libs/glad/src/glad.c
)

# Add custom CMake module paths

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)
find_package(EXPAT REQUIRED)

# Include directories
target_include_directories(route_tracer PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    libs/libosmium/include
    libs/protozero/include
    libs/glad/include
)

# Link libraries
target_link_libraries(route_tracer PRIVATE
    glfw
    OpenGL::GL
    X11::X11
    ZLIB::ZLIB
    BZip2::BZip2
    EXPAT::EXPAT
)