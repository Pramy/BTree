cmake_minimum_required(VERSION 3.9)
project(demo)

set(CMAKE_CXX_STANDARD 11)

add_executable(demo main.cpp main.cpp delete.h constant.h delete.cpp insert.h insert.cpp structure.h structure.cpp search.h search.cpp btree.h)