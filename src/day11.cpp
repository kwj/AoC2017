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
#include <functional>
#include <istream>
#include <ranges>
#include <string_view>
#include <tuple>
#include <vector>

import util;

// --------
export module day11;

export namespace day11 {

class [[nodiscard]] HexPos {
  public:
    HexPos(long _a, long _b, long _c) : a(_a), b(_b), c(_c) {};
    HexPos operator+(HexPos const &other) const;
    HexPos &operator+=(HexPos const &other);
    [[nodiscard]] long distance() const;

  private:
    long a;
    long b;
    long c;
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
HexPos::operator+(HexPos const &other) const {
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
HexPos::distance() const {
    return (std::abs(a) + std::abs(b) + std::abs(c)) / 2;
}

std::vector<HexPos>
parse(std::istream &is) {
    auto &&line = util::getLine(is);
    std::string_view const s {line};

    std::vector<HexPos> dirs;
    for (auto const &subr : std::views::split(s, ',')) {
        auto d = std::string_view(subr);
        if (d == "n") {
            dirs.emplace_back(0, 1, -1);
        } else if (d == "ne") {
            dirs.emplace_back(1, 0, -1);
        } else if (d == "se") {
            dirs.emplace_back(1, -1, 0);
        } else if (d == "s") {
            dirs.emplace_back(0, -1, 1);
        } else if (d == "sw") {
            dirs.emplace_back(-1, 0, 1);
        } else if (d == "nw") {
            dirs.emplace_back(-1, 1, 0);
        }
    }

    return dirs;
}

long
part1(std::vector<HexPos> const &dirs) {
    HexPos const origin {0, 0, 0};
    auto goal = std::ranges::fold_left(dirs, origin, std::plus<HexPos> {});

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
