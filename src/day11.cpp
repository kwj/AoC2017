/*
hexagonal position: [a, b, c]

    [+B]   [-C]
       \ n  /
     nw +--+ ne
       / A  \
[-A] -+      +- [+A]
       \B  C/
     sw +--+ se
       / s  \
    [+C]    [-B]
*/

module;

#include <algorithm>
#include <cmath>
#include <istream>
#include <ranges>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

import util;

// --------
export module day11;

export namespace day11 {

struct HexPos {
    long a;
    long b;
    long c;
    HexPos operator+(HexPos const &other);
    HexPos &operator+=(HexPos const &other);
    long distance();
};

std::tuple<long, long> solve(std::istream &is);
std::vector<HexPos> parse(std::istream &is);
long part1(std::vector<HexPos> const &dirs);
long part2(std::vector<HexPos> const &dirs);

} // namespace day11

// --------
module :private;

namespace day11 {

std::tuple<long, long>
solve(std::istream &is) {
    auto dirs = parse(is);

    return {part1(dirs), part2(dirs)};
}

HexPos
HexPos::operator+(HexPos const &other) {
    return {a + other.a, b + other.b, c + other.c};
}

HexPos &
HexPos::operator+=(HexPos const &other) {
    a += other.a;
    b += other.b;
    c += other.c;

    return *this;
}

long
HexPos::distance() {
    return (std::abs(a) + std::abs(b) + std::abs(c)) / 2;
}

std::vector<HexPos>
parse(std::istream &is) {
    auto &&line = util::getLine(is);
    std::string_view s {line};

    std::vector<HexPos> dirs;
    for (auto const &subr : std::views::split(s, ',')) {
        auto dir = std::string_view(subr);
        if (dir == "n") {
            dirs.push_back({0, 1, -1});
        } else if (dir == "ne") {
            dirs.push_back({1, 0, -1});
        } else if (dir == "se") {
            dirs.push_back({1, -1, 0});
        } else if (dir == "s") {
            dirs.push_back({0, -1, 1});
        } else if (dir == "sw") {
            dirs.push_back({-1, 0, 1});
        } else if (dir == "nw") {
            dirs.push_back({-1, 1, 0});
        }
    }

    return dirs;
}

long
part1(std::vector<HexPos> const &dirs) {
    HexPos origin {0, 0, 0};
    auto goal =
        std::ranges::fold_left(dirs, origin, [](HexPos lhs, HexPos const &rhs) {
            return lhs + rhs;
        });

    return goal.distance();
}

long
part2(std::vector<HexPos> const &dirs) {
    HexPos pos {0, 0, 0};
    long max_distance {0};

    for (auto const &d : dirs) {
        pos += d;
        max_distance = std::max(max_distance, pos.distance());
    }

    return max_distance;
}

} // namespace day11
