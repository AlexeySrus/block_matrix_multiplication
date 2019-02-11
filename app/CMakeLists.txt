cmake_minimum_required(VERSION 3.8)
project(block_matrix_multiplication)

set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm/bin/clang++")

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-O3 -fopenmp -L/usr/local/opt/llvm/lib")

set(SOURCE_FILES main.cpp ../libs/block.cpp ../libs/block.h ../libs/matrix.cpp ..//libs/matrix.h ../libs/logging.cpp ../libs/logging.h ../configuration/config.h)
add_executable(block_matrix_multiplication ${SOURCE_FILES} ../libs/matrix.cpp ../libs/matrix.h ../libs/logging.cpp ../libs/logging.h ../configuration/config.h)