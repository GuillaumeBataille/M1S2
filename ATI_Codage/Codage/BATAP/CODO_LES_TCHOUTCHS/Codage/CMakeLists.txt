cmake_minimum_required(VERSION 3.24)
project(Codage)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(Codage
        src/color.h
        src/image_ppm.h
        src/q4.cpp
        src/ImageAlgorithms.h)
