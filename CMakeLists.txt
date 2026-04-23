

cmake_minimum_required(VERSION 3.10)
project(any_ptr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable for testing
add_executable(test1 test1.cpp)
add_executable(test2 test2.cpp)

# Main executable for OJ
add_executable(code main.cpp)

