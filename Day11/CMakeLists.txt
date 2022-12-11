cmake_minimum_required(VERSION 3.20)
project(Day11)

set(CMAKE_CXX_STANDARD 17)

set(Assets Example.txt)
file(COPY ${Assets} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(Day11 main.cpp)
