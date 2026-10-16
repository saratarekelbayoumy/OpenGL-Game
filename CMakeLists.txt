
cmake_minimum_required(VERSION 3.16)

project(OpenGLGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(OpenGLGame
    Template/main.cpp
    Template/src/glad.c
)

target_include_directories(OpenGLGame PRIVATE
    Template
    Template/include
    Template/External
)

target_link_libraries(OpenGLGame PRIVATE
    glfw
    OpenGL::GL
)
