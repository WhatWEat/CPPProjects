cmake_minimum_required(VERSION 3.23)
project(Projects)

set(CMAKE_CXX_STANDARD 14)

add_executable(Projects main.cpp)
add_subdirectory(Project1)
add_subdirectory(Project2)
add_subdirectory(Project3)
add_subdirectory(Project4)
add_subdirectory(Project5)