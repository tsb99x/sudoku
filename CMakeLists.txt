cmake_minimum_required(VERSION 3.13)
project(sudoku LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

enable_testing()

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-pedantic -Wall -Wextra -Werror)
endif()

add_subdirectory(lib)
add_subdirectory(src)
