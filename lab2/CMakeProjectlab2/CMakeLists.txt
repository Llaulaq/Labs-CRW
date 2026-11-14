cmake_minimum_required(VERSION 3.20)
project(CMakeProjectLab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(CMakeProjectLab2 CMakeProjectLab2.cpp)
target_link_libraries(CMakeProjectLab2 PRIVATE Threads::Threads)

if (MSVC)
    target_compile_options(CMakeProjectLab2 PRIVATE /W4)
endif()