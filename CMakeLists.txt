cmake_minimum_required(VERSION 3.7)
project(c_playground)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c list.h utility.h linkedlist.c list_tests.c)
add_executable(c_playground ${SOURCE_FILES})