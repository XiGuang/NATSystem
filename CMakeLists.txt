cmake_minimum_required(VERSION 3.20)
project(NATSystem)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)

add_executable(NATSystem ${SRC})
