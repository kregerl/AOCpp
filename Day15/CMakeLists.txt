cmake_minimum_required(VERSION 3.20)
project(Day15)

set(CMAKE_CXX_STANDARD 17)

set(Assets Example.txt Input1.txt)
file(COPY ${Assets} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(Day15 main.cpp main.cpp)
