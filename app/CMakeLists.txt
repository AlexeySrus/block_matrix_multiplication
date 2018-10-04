cmake_minimum_required(VERSION 3.8)
project(block_matrix_multiplication)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-O3")

set(SOURCE_FILES main.cpp ../libs/block.cpp ../libs/block.h ../libs/matrix.cpp ..//libs/matrix.h)
add_executable(block_matrix_multiplication ${SOURCE_FILES} ../libs/matrix.cpp ../libs//matrix.h)