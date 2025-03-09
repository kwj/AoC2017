cmake_minimum_required(VERSION 3.30)

project(AoC2017 CXX)

add_compile_options(
    -stdlib=libc++
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow-all
    -Werror
)

add_link_options(
    -stdlib=libc++
)

add_subdirectory(src)
