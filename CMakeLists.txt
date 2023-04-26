cmake_minimum_required(VERSION 3.13.4)
project(coroutine_demo)

set(CMAKE_CXX_STANDARD 11)

add_executable(coroutine_demo main.cpp coroutine_thread.h coroutine_thread.cpp)
