cmake_minimum_required(VERSION 3.7)
project(oskar)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp OskarCube.cpp OskarCube.h)
add_executable(oskar ${SOURCE_FILES})