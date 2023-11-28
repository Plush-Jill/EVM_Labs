cmake_minimum_required(VERSION 3.26)
project(Lab_7)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab_7 main.cpp
        MatrixBLAS.cpp
        MatrixBLAS.h
        MatrixNaive.cpp
        MatrixNaive.h
        MatrixVectorize.cpp
        MatrixVectorize.h)
