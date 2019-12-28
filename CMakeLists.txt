cmake_minimum_required(VERSION 3.14)
project(snake)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-lncurses -pthread")

add_executable(snake main.cpp winsys.cpp screen.cpp snake.cpp)