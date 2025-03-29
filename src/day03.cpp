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

#include <array>
#include <cmath>
#include <complex>
#include <cstddef>
#include <functional>
#include <istream>
#include <string>
#include <tuple>
#include <unordered_map>

import util;

// --------
export module day03;

export namespace day03 {

std::tuple<long, long> solve(std::istream &is);
long parse(std::istream &is);
long part1(long target);
long part2(long target);

} // namespace day03

// --------
module :private;

namespace day03 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

long
parse(std::istream &is) {
    return std::stol(util::getLine(is));
}

inline long
ring_max_number(long n) {
    return (2 * n + 1) * (2 * n + 1);
}

long
part1(long target) {
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
        auto h = std::hash<long> {};
        auto r = static_cast<long>(key.real());
        auto i = static_cast<long>(key.imag());

        return h((r & 0xFFFF) << 16 | (i & 0xFFFF));
    }
};

using Pos = std::complex<int>;
using Grid = std::unordered_map<Pos, long, ComplexHash>;

long
fixValue(Grid &m, Pos const &pos) {
    constexpr auto dirs = std::to_array<Pos>({
        {1, 0},
        {1, 1},
        {0, 1},
        {-1, 1},
        {-1, 0},
        {-1, -1},
        {0, -1},
        {1, -1},
    });

    long result {0};
    for (auto const &d : dirs) {
        // If m[pos + d] doesn't exist, it is initialized to 0.
        result += m[pos + d];
    }

    return result;
}

long
part2(long target) {
    auto turnLeft90 = [](Pos const &d) { return d * Pos {0, 1}; };

    Grid grid = {{{0, 0}, 1}};
    Pos curr_pos {0, 0};
    Pos dir {1, 0};
    int i {0};
    int dist {1};

    while (grid[curr_pos] <= target) {
        curr_pos += dir;
        grid[curr_pos] = fixValue(grid, curr_pos);

        if (++i == dist) {
            i = 0;
            dir = turnLeft90(dir);
            if (dir.real() != 0) {
                ++dist;
            }
        }
    }

    return grid[curr_pos];
}

} // namespace day03
