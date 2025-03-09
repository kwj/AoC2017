cmake_minimum_required(VERSION 3.30)

project(AoC2017 CXX)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow-all
    -Werror
)

add_subdirectory(src)
