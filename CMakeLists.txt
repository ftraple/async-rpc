cmake_minimum_required(VERSION 3.10)

project(arpc VERSION 0.0.0)

option(BUILD_LIBRARY "Build the arpc library." ON)
option(BUILD_TEST "Build the tests." ON)

if (BUILD_LIBRARY)
    add_subdirectory(arpc)
endif()
if(BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
