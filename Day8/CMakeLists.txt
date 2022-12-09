cmake_minimum_required(VERSION 3.20)
project(Day8)

set(CMAKE_CXX_STANDARD 17)

set(Assets Example.txt Input1.txt)
file(COPY ${Assets} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(Day8 main.cpp main.cpp main.cpp main.cpp)
