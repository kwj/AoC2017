module;

#include <chrono>
#include <format>
#include <fstream>
#include <functional>
#include <map>
#include <print>
#include <sstream>
#include <string_view>
#include <tuple>

import day01;
import day02;
import day03;
import day04;
import day05;
import day06;
import day07;
import day08;
import day09;
import day10;
import day11;
import day12;
import day13;
import day14;
import day15;
import day16;
import day17;
import day18;
import day19;
import day20;
import day21;
import day22;
import day23;

// --------
export module solver;

export namespace solver {

void runAllSolver();
void runSolver(int day, std::string fpath = "");

} // namespace solver

// --------
module :private;

namespace solver {

std::stringstream getInput(std::string const &fpath) {
    std::ifstream fin(fpath);
    if (!fin) {
        throw std::ios_base::failure{"File opening failed"};
    }

    std::stringstream buf;
    buf << fin.rdbuf();

    return buf;
}

std::tuple<int, int, std::string> convertTime(std::chrono::steady_clock::rep &&x) {
    constexpr std::array units{"s", "ms", "μs", "ns"};

    std::size_t idx{units.size() - 1};
    while (idx > 1 && x >= 1'000'000) {
        --idx;
        x /= 1'000;
    }

    if (x < 1'000) {
        return {x, 0, units[idx]};
    } else {
        return {x / 1'000, x % 1'000, units[idx - 1]};
    }
}

template <auto solveFn, int day>
void solver(std::string fpath) {
    if (fpath.size() == 0) {
        fpath = std::format("./inputs/{0:02d}/input", day);
    }

    auto const t0 = std::chrono::steady_clock::now();
    auto input_ss = getInput(fpath);
    auto const tpl = solveFn(input_ss);
    auto const t1 = std::chrono::steady_clock::now();

    auto [n1, n2, unit] = convertTime(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());

    std::print("[Day {}]\n", day);
    std::apply([](auto &&...args){((std::print("{}\n", args)), ...);}, tpl);
    if (n2 == 0) {
        std::print("Elapsed time: {}{}\n\n", n1, unit);
    } else {
        std::print("Elapsed time: {}.{:03d}{}\n\n", n1, n2, unit);
    }

    return;
}

std::map<int, std::function<void(std::string)>> Solvers{
    {1, solver<&day01::solve, 1>},
    {2, solver<&day02::solve, 2>},
    {3, solver<&day03::solve, 3>},
    {4, solver<&day04::solve, 4>},
    {5, solver<&day05::solve, 5>},
    {6, solver<&day06::solve, 6>},
    {7, solver<&day07::solve, 7>},
    {8, solver<&day08::solve, 8>},
    {9, solver<&day09::solve, 9>},
    {10, solver<&day10::solve, 10>},
    {11, solver<&day11::solve, 11>},
    {12, solver<&day12::solve, 12>},
    {13, solver<&day13::solve, 13>},
    {14, solver<&day14::solve, 14>},
    {15, solver<&day15::solve, 15>},
    {16, solver<&day16::solve, 16>},
    {17, solver<&day17::solve, 17>},
    {18, solver<&day18::solve, 18>},
    {19, solver<&day19::solve, 19>},
    {20, solver<&day20::solve, 20>},
    {21, solver<&day21::solve, 21>},
    {22, solver<&day22::solve, 22>},
    {23, solver<&day23::solve, 23>},
};

void runAllSolver() {
    for (auto const &[_, fn] : Solvers) {
        fn("");
    }

    return;
}

void runSolver(int day, std::string fpath) {
    if (Solvers.contains(day)) {
        Solvers[day](fpath);
    } else {
        std::print("There is no solution for Day {}\n", day);
    }

    return;
}

} // namespace solver
