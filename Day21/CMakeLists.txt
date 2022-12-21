cmake_minimum_required(VERSION 3.20)
project(Day21)

set(CMAKE_CXX_STANDARD 20)

set(Assets Example.txt Input1.txt)
file(COPY ${Assets} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(Day20 main.cpp)