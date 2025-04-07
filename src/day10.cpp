module;

#include <algorithm>
#include <array>
#include <concepts>
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
std::string part2(std::string_view s);

} // namespace day10

// --------
module :private;

namespace day10 {

constexpr unsigned long BLOCK_LEN {16};
constexpr unsigned long KNOTS_LEN {BLOCK_LEN * 16};

std::tuple<long, std::string>
solve(std::istream &is) {
    auto result = parse(is);

    return {part1(result), part2(result)};
}

std::string
parse(std::istream &is) {
    return util::getLine(is);
}

std::array<long, KNOTS_LEN>
knotHash(std::vector<unsigned long> lengths, long round) {
    std::array<long, KNOTS_LEN> knots;

// P2440R1 (ranges::iota) is not yet supported in libc++ 19.
#if __cpp_lib_ranges_iota >= 202202L
    std::ranges::iota(knots, 0);
#else
    std::iota(knots.begin(), knots.end(), 0);
#endif

    unsigned long skip {0};
    unsigned long pos {0};
    while (round-- > 0) {
        for (auto const len : lengths) {
            // reverse the order by swapping elements
            auto x = pos;
            auto y = (pos + len - 1) % KNOTS_LEN;
            for (auto cnt = 0ul; cnt < len / 2; ++cnt) {
                std::ranges::swap(knots[x], knots[y]);
                x = (x + 1) % KNOTS_LEN;
                y = y == 0 ? KNOTS_LEN - 1 : y - 1;
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
part2(std::string_view s) {
    std::vector<unsigned long> const tail {17, 31, 73, 47, 23};
    auto input_view = std::views::transform(s, [](auto &ch) {
        return static_cast<unsigned long>(ch);
    });

// P1206R7 (range to container conversion) is not yet fully supported in GCC 14.
#if __cpp_lib_containers_ranges >= 202202L
    std::vector<unsigned long> lengths(std::from_range, input_view);
    lengths.append_range(tail);
#else
    std::vector<unsigned long> lengths(input_view.begin(), input_view.end());
    lengths.insert(lengths.end(), tail.cbegin(), tail.cend());
#endif

    auto hash = knotHash(lengths, 64);
    std::ostringstream oss;
    for (auto it = hash.cbegin(); it <= hash.cend() - BLOCK_LEN;
         std::ranges::advance(it, BLOCK_LEN)) {
        auto h = std::ranges::fold_left(
            std::ranges::subrange(it, it + BLOCK_LEN), 0, std::bit_xor<long> {}
        );
        oss << std::format("{:02x}", h & 0xFF);
    }

    return oss.str();
}

} // namespace day10
