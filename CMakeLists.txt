cmake_minimum_required(VERSION 3.6)
project(mi_run_gc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    mas.c)

add_executable(mi_run_gc ${SOURCE_FILES})
add_definitions(-std=c11)