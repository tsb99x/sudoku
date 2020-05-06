cmake_minimum_required(VERSION 3.10)
project(sudoku LANGUAGES C)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(SDL)
enable_testing()
add_subdirectory(src)
