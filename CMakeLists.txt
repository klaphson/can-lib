cmake_minimum_required(VERSION 3.0.0)
project(can-lib VERSION 0.1.0)

add_library(${PROJECT_NAME} 
    src/can-lib.cpp
    src/can-package.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)
