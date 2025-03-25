cmake_minimum_required(VERSION 3.30)

project(AoC2017 CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# I want to use libc++ on Ubuntu with GNU libstdc++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

add_compile_options(
    -O2
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow-all
    -Wdouble-promotion
    -Werror
)

add_subdirectory(src)
