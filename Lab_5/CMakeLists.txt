cmake_minimum_required(VERSION 3.22.1)
project(Lab_5)

set(CMAKE_CXX_STANDARD 20)
find_package(OpenCV REQUIRED)
include_directories ("/usr/include/opencv4/")
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(Lab_5 main.cpp
        Lab_5_Exceptions.cpp
        Lab_5_Exceptions.h)

target_link_libraries(Lab_5 ${OpenCV_LIBS})


