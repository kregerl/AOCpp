cmake_minimum_required(VERSION 3.20)
project(Day9)

set(CMAKE_CXX_STANDARD 17)

set(Assets Example.txt Example2.txt Input1.txt)
file(COPY ${Assets} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(Day9 main.cpp main.cpp main.cpp main.cpp)
