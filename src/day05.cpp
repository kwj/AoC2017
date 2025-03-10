module;

#include <functional>
#include <sstream>
#include <string>
#include <vector>

import util;

// --------
export module day05;

export namespace day05 {

std::tuple<long, long> solve(std::stringstream &ss);
std::vector<long> parse(std::stringstream &ss);
long part1(std::vector<long> const &vvs);
long part2(std::vector<long> const &vvs);

}  // namespace day05

// --------
module :private;

namespace day05 {

std::tuple<long, long> solve(std::stringstream &ss) {
    auto input_data = parse(ss);

    return {part1(input_data), part2(input_data)};
}

std::vector<long> parse(std::stringstream &ss) {
    std::vector<long> result;

    for (std::string line; std::getline(ss, line);) {
        result.push_back(std::stol(line));
    }

    return result;
}

long run(std::vector<long> insts, std::function<long(long)> f) {
    long steps{0};
    size_t idx{0};
    auto len{insts.size()};

    while (idx >= 0 && idx < len) {
        auto offset = insts[idx];
        insts[idx] = f(insts[idx]);
        idx = static_cast<size_t>(static_cast<long>(idx) + offset);
        ++steps;
    }

    return steps;
}

long part1(std::vector<long> const &vn) {
    std::vector<long> instructions{vn};
    std::function<long(long)> f = [](long n){ return n + 1; };

    return run(instructions, f);
}

// It takes a few seconds on Raspberry Pi 4.
// If we want to try to improve performance, we need to analyze the input data.
long part2(std::vector<long> const &vn) {
    std::vector<long> instructions{vn};
    std::function<long(long)> f = [](long n){ return n >= 3 ? n - 1 : n + 1; };

    return run(instructions, f);
}

} // namespace day05
