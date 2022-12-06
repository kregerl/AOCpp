cmake_minimum_required(VERSION 3.20)
project(Day6)

set(CMAKE_CXX_STANDARD 17)

set(Assets Example1.txt Example2.txt Example3.txt Example4.txt Example5.txt Input1.txt)
file(COPY ${Assets} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(Day6 main.cpp main.cpp main.cpp)
