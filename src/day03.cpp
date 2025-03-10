/*
[Part 1]
       ring = 0,  1,  2,  3, ...
              v
 37  36  35  34  33  32  31
 38  17  16  15  14  13  30
 39  18   5   4   3  12  29
 40  19   6  [1]  2  11  28
 41  20   7   8  [9] 10  27
 42  21  22  23  24 [25] 26  ..
 43  44  45  46  47  48 [49] 50
------------------------------

  0   5   4   3   2   1   0
  1   0   3   2   1   0   5
  2   1   0   1   0   3   4
  3   2   1   0   1   2   3
  4   3   0   1   0   1   2
  5   0   1   2   3   0   1  ..
  0   1   2   3   4   5   0   1
*/

module;

#include <cmath>
#include <complex>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <array>

import util;

// --------
export module day03;

export namespace day03 {

std::tuple<long, long> solve(std::stringstream &ss);
long parse(std::stringstream &ss);
long part1(long target);
long part2(long target);

} // namespace day03

// --------
module :private;

namespace day03 {

std::tuple<long, long> solve(std::stringstream &ss) {
    auto input_data = parse(ss);

    return {part1(input_data), part2(input_data)};
}

long parse(std::stringstream &ss) {
    return std::stol(util::getLine(ss));
}

inline long ring_max_number(long n) {
    return (2 * n + 1) * (2 * n + 1);
}

long part1(long target) {
    if (target == 1) {
        return 0;
    } else {
        auto ring = std::lround(std::ceil(std::sqrt(target))) / 2;
        auto base = ring_max_number(ring - 1);
        auto offset = (target - base) % (2 * ring);

        return ring + std::abs(ring - offset);
    }
}

struct ComplexHash {
    std::size_t operator()(std::complex<int> const &key) const {
        auto r = static_cast<long>(key.real());
        auto i = static_cast<long>(key.imag());

        return std::hash<long>{}((r & 0xFFFF) << 16 | (i & 0xFFFF));
    }
};

long fixValue(std::unordered_map<std::complex<int>, long, ComplexHash> &m, std::complex<int> const &pos) {
    std::array<std::complex<int>, 8> dirs = {{
        {1, 0},
        {1, 1},
        {0, 1},
        {-1, 1},
        {-1, 0},
        {-1, -1},
        {0, -1},
        {1, -1},
    }};

    long result{0};
    for (auto const d : dirs) {
        result += m[pos + d];  // If m[pos + d] doesn't exist, it is initialized to 0.
    }

    return result;
}

long part2(long target) {
    // std::complex has the == operator, so use it.
    std::unordered_map<std::complex<int>, long, ComplexHash> grid = {{{0, 0}, 1}};

    std::complex<int> pos(0, 0);
    std::complex<int> dir(1, 0);
    int i{0};
    int dist{1};

    while (grid[pos] <= target) {
        pos += dir;
        grid[pos] = fixValue(grid, pos);

        if (++i == dist) {
            i = 0;
            dir *= std::complex<int>{0, 1};
            if (dir.real() != 0) {
                ++dist;
            }
        }
    }

    return grid[pos];
}

}  // module day03

