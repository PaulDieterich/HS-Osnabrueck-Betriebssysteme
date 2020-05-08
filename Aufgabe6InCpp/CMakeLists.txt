cmake_minimum_required(VERSION 3.16)
project(Aufgabe6InCpp)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}  " -std=c++11 -pthread")

add_executable(Aufgabe6InCpp main.cpp)