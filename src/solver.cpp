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

    int idx{units.size() - 1};
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
