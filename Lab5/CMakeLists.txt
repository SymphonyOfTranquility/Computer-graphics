cmake_minimum_required(VERSION 3.15)
include_directories(SYSTEM ./lib)
project(Lab5)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab5 main.cpp QuickHull.cpp QuickHull.h)