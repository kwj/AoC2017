module;

#include <algorithm>
#include <format>
#include <functional>
#include <istream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

import util;

// --------
export module day10;

export namespace day10 {

std::tuple<long, std::string> solve(std::istream &is);
std::string parse(std::istream &is);
long part1(std::string_view s);
std::string part2(std::string const &s);

} // namespace day10

// --------
module :private;

namespace day10 {

constexpr unsigned long KNOTS_LEN {256};

std::tuple<long, std::string>
solve(std::istream &is) {
    auto result = parse(is);

    return {part1(result), part2(result)};
}

std::string
parse(std::istream &is) {
    return util::getLine(is);
}

std::vector<long>
knotHash(std::vector<unsigned long> lengths, long round) {
    auto knots = std::vector<long>(KNOTS_LEN);

// P2440R1 (ranges::iota) is not yet supported in libc++ 19.
#if __cpp_lib_ranges_iota
    std::ranges::iota(knots, 0);
#else
    std::iota(knots.begin(), knots.end(), 0);
#endif

    unsigned long skip {0};
    unsigned long pos {0};
    while (round-- > 0) {
        for (auto const len : lengths) {
            // reverse
            auto i1 = pos;
            auto i2 = (pos + len - 1) % KNOTS_LEN;
            for (unsigned long cnt = 0; cnt < len / 2; ++cnt) {
                std::swap(knots[i1], knots[i2]);
                i1 = (i1 + 1) % KNOTS_LEN;
                i2 = i2 > 0 ? i2 - 1 : KNOTS_LEN - 1;
            }

            // move position
            pos = (pos + len + skip) % KNOTS_LEN;
            ++skip;
        }
    }

    return knots;
}

long
part1(std::string_view s) {
    auto hashed_data = knotHash(
        std::ranges::to<std::vector<unsigned long>>(util::getNumbers(s)), 1
    );

    return hashed_data[0] * hashed_data[1];
}

std::string
part2(std::string const &s) {
    std::vector<unsigned long> lengths;
    std::vector<unsigned long> const tail {17, 31, 73, 47, 23};
    std::istringstream iss(s); // istringstream accepts string_view SINCE C++26
    char ch;

    while (iss.get(ch)) {
        lengths.push_back(static_cast<unsigned long>(ch));
    }
    lengths.append_range(tail);

    auto hash = knotHash(lengths, 64);
    std::ostringstream oss;
    for (auto it = hash.cbegin(); it < hash.cend(); std::advance(it, 16)) {
        auto h = std::ranges::fold_left(
            std::ranges::subrange(it, it + 16), 0, std::bit_xor<long> {}
        );
        oss << std::format("{:02x}", h & 0xFF);
    }

    return oss.str();
}

} // namespace day10
