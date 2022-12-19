cmake_minimum_required(VERSION 3.20)
project(Day19)

set(CMAKE_CXX_STANDARD 20)

set(Assets Example.txt Input1.txt)
file(COPY ${Assets} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(Day19 main.cpp SimpleParser.h)
target_link_libraries(Day19 pthread)
