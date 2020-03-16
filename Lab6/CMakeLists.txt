cmake_minimum_required(VERSION 3.15)
include_directories(SYSTEM ./lib)
project(Lab6)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab6 main.cpp GiftWrapping.cpp GiftWrapping.h)