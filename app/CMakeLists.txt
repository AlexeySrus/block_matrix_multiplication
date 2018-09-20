cmake_minimum_required(VERSION 3.8)
project(block_matrix_multiplication)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp ../libs/block.cpp ../libs/block.h)
add_executable(block_matrix_multiplication ${SOURCE_FILES})