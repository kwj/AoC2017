add_library(solvers)
target_include_directories(solvers PRIVATE .)
target_sources(
    solvers
    PUBLIC
    FILE_SET solvers_modules
    TYPE CXX_MODULES
    FILES
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
        day20.cpp
        day21.cpp
        day22.cpp
        day23.cpp
        day24.cpp
        day25.cpp
        solver.cpp
        util.cpp
)

add_executable(aoc2017)
target_sources(aoc2017 PRIVATE main.cpp)
target_link_libraries(aoc2017 PRIVATE solvers)
