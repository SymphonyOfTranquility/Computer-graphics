cmake_minimum_required(VERSION 3.15)
include_directories(SYSTEM ./lib)
project(Lab4)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab4 main.cpp RegionTree.cpp RegionTree.h)