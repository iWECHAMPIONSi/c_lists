cmake_minimum_required(VERSION 3.30)
project(example C)

set(CMAKE_C_STANDARD 23)

add_executable(example main.c
        list_int.c
        list_int.h
        )
