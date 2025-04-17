/*
These linear congruential generators are known as MINSTD, and emit pseudo-randomized numbers.
Their implementations exist in C++11 or later, however, I did not use them this time.

Lehmer random number generator
https://en.wikipedia.org/wiki/Lehmer_random_number_generator
*/

module;

#include <istream>
#include <ranges>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

import util;

// --------
export module day15;

export namespace day15 {

std::tuple<long, long> solve(std::istream &is);
std::pair<long, long> parse(std::istream &is);
long part1(std::pair<long, long> const &result);
long part2(std::pair<long, long> const &result);

} // namespace day15

// --------
module :private;

namespace day15 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

std::pair<long, long>
judgement(unsigned long a, unsigned long b) {
    constexpr long P1_N_PAIRS {40'000'000};
    constexpr long P2_N_PAIRS {5'000'000};
    constexpr unsigned long A {16'807};
    constexpr unsigned long B {48'271};
    constexpr unsigned long M {0x7FFF'FFFF};

    // This method stores 10 million values, and then checks them.
    // (2 * P2_N_PAIRS * unsigned short -> at least 20MB)
    // If there is not enough memory, it needs to use a different algorithm.
    std::vector<unsigned short> mul4;
    std::vector<unsigned short> mul8;
    mul4.reserve(P2_N_PAIRS);
    mul8.reserve(P2_N_PAIRS);

    long p1_cnt {0};
    long m4_cnt {0};
    long m8_cnt {0};

    long judge_p1 {0};
    while (p1_cnt < P1_N_PAIRS || m4_cnt < P2_N_PAIRS || m8_cnt < P2_N_PAIRS) {
        a = (a * A) % M;
        b = (b * B) % M;

        auto a_16 = a & 0xFFFF;
        auto b_16 = b & 0xFFFF;
        if (p1_cnt < P1_N_PAIRS && a_16 == b_16) {
            ++judge_p1;
        }
        if (m4_cnt < P2_N_PAIRS && (a_16 & 0b11) == 0) {
            mul4.push_back(static_cast<unsigned short>(a_16));
            ++m4_cnt;
        }
        if (m8_cnt < P2_N_PAIRS && (b_16 & 0b111) == 0) {
            mul8.push_back(static_cast<unsigned short>(b_16));
            ++m8_cnt;
        }

        ++p1_cnt;
    }

    long judge_p2 {0};
    for (auto const [x, y] : std::views::zip(mul4, mul8)) {
        if (x == y) {
            ++judge_p2;
        }
    }

    return {judge_p1, judge_p2};
}

std::pair<long, long>
parse(std::istream &is) {
    std::vector<unsigned long> seeds;
    for (std::string line; std::getline(is, line);) {
        auto nums = util::getNumbers(line);
        seeds.push_back(static_cast<unsigned long>(nums[0]));
    }

    return judgement(seeds[0], seeds[1]);
}

long
part1(std::pair<long, long> const &result) {
    return result.first;
}

long
part2(std::pair<long, long> const &result) {
    return result.second;
}

} // namespace day15
