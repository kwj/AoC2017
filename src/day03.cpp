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
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <complex>

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

long getValue(std::vector<std::vector<long>> const &m, std::complex<int> pos) {
    auto x = static_cast<unsigned int>(pos.real());
    auto y = static_cast<unsigned int>(pos.imag());

    return m[y][x];
}

void setValue(std::vector<std::vector<long>> &m, std::complex<int> pos, long v) {
    auto x = static_cast<unsigned int>(pos.real());
    auto y = static_cast<unsigned int>(pos.imag());
    m[y][x] = v;

    return;
}

long fixValue(std::vector<std::vector<long>> const &m, std::complex<int> pos) {
    std::array<std::complex<int>, 8> dirs{
        std::complex<int>{1, 0},
        std::complex<int>{1, 1},
        std::complex<int>{0, 1},
        std::complex<int>{-1, 1},
        std::complex<int>{-1, 0},
        std::complex<int>{-1, -1},
        std::complex<int>{0, -1},
        std::complex<int>{1, -1},
    };

    long result{0};
    for (auto d : dirs) {
        result += getValue(m, pos + d);
    }

    return result;
}

long part2(long target) {
    auto size{static_cast<unsigned int>(std::lround(std::ceil(std::sqrt(target))) + 2)};
    std::vector<std::vector<long>> matrix(size, std::vector<long>(size, 0));

    std::complex<int> pos(size / 2, size / 2);
    std::complex<int> dir(1, 0);
    matrix[size / 2][size / 2] = 1;
    int i{0};
    int dist{1};

    while (getValue(matrix, pos) <= target) {
        pos += dir;
        setValue(matrix, pos, fixValue(matrix, pos));

        if (++i == dist) {
            i = 0;
            dir *= std::complex<int>{0, 1};
            if (dir.real() != 0) {
                ++dist;
            }
        }
    }

    return getValue(matrix, pos);
}

}  // module day03

