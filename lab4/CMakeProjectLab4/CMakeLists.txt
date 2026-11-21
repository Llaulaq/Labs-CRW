cmake_minimum_required(VERSION 3.20)
project(CMakeProjectLab4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(CMakeProjectLab4 CMakeProjectLab4.cpp)
target_link_libraries(CMakeProjectLab4 PRIVATE Threads::Threads)

if (MSVC)
    target_compile_options(CMakeProjectLab4 PRIVATE /W4 /utf-8)
endif()
