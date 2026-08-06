cmake_minimum_required(VERSION 3.16)

project(my-render)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB_RECURSE SOURCES
    main.cpp
    src/*.c
)

add_executable(my-render ${SOURCES})

target_include_directories(my-render PRIVATE
    include
)

target_link_libraries(my-render PRIVATE
    glfw
    OpenGL::GL
)