cmake_minimum_required(VERSION 3.15)
include_directories(SYSTEM ./lib)
project(Lab8)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab8 main.cpp ApproximateConvex.cpp ApproximateConvex.h)