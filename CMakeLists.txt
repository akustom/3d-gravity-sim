cmake_minimum_required(VERSION 4.1)
project(3grav)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)

add_executable(3grav
        src/main.cpp
        src/glad.c
        include/gfx/camera.cpp
        include/gfx/mesh.cpp
)

target_include_directories(3grav PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(3grav ${GLFW3_LIBRARY} OpenGL::GL)

target_compile_definitions(3grav PRIVATE
        SOURCE_DIR="${CMAKE_SOURCE_DIR}/"
)