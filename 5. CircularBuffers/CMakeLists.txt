cmake_minimum_required(VERSION 4.1)
project(5__CircularBuffers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(5__CircularBuffers
        circular_buffers.cpp)
