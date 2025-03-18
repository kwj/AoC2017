add_library(solvers)
target_compile_features(solvers PUBLIC cxx_std_23)
target_include_directories(solvers PRIVATE .)
target_sources(
    solvers
    PUBLIC
    FILE_SET solvers_modules
    TYPE CXX_MODULES
    FILES
        util.cpp
        day01.cpp
        day02.cpp
        day03.cpp
        day04.cpp
        day05.cpp
        day06.cpp
        day07.cpp
        day08.cpp
        day09.cpp
        day10.cpp
        day11.cpp
        day12.cpp
        day13.cpp
        day14.cpp
        day15.cpp
        day16.cpp
        day17.cpp
        day18.cpp
        day19.cpp
        solver.cpp
)

add_executable(aoc2017)
target_compile_features(aoc2017 PUBLIC cxx_std_23)
target_sources(aoc2017 PRIVATE main.cpp)
target_link_libraries(aoc2017 PRIVATE solvers)
