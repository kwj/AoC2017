module;

#include <algorithm>
#include <format>
#include <functional>
#include <istream>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

import util;

constexpr long KNOTS_LEN{256};

// --------
export module day10;

export namespace day10 {

std::tuple<long, std::string> solve(std::istream &is);
std::string parse(std::istream &is);
long part1(std::string const &s);
std::string part2(std::string const &s);

}  // namespace day10

// --------
module :private;

namespace day10 {

std::tuple<long, std::string> solve(std::istream &is) {
    auto result = parse(is);

    return {part1(result), part2(result)};
}

std::string parse(std::istream &is) {
    return util::getLine(is);
}

std::vector<long> knotHash(std::vector<long> lengths, long cnt) {
    auto knots = std::vector<long>(KNOTS_LEN);
    std::iota(knots.begin(), knots.end(), 0);

    long skip{0};
    long origin{0};
    for (auto round = 0; round < cnt; ++round) {
        for (auto const len : lengths) {
            std::reverse(knots.begin(), knots.begin() + len);

            auto offset = (len + skip) % KNOTS_LEN;
            std::rotate(knots.begin(), knots.begin() + offset, knots.end());
            origin += offset;
            ++skip;
        }
    }

    std::rotate(knots.begin(), knots.end() - (origin % KNOTS_LEN), knots.end());

    return knots;
}

long part1(std::string const &s) {
    auto hashed_data = knotHash(util::getNumbers(s), 1);

    return hashed_data[0] * hashed_data[1];
}

std::string part2(std::string const &s) {
    std::vector<long> lengths;
    std::vector<long> const tail{17, 31, 73, 47, 23};
    std::istringstream iss(s);
    char ch;

    while (iss.get(ch)) {
        lengths.push_back(static_cast<long>(ch));
    }
    lengths.append_range(tail);

    auto hashed_data = knotHash(lengths, 64);
    std::string hash_str;
    for (auto i = 0; i < KNOTS_LEN; i += 16) {
        auto h = std::ranges::fold_left(hashed_data.begin() + i, hashed_data.begin() + i + 16, 0, std::bit_xor<long>());
        hash_str.append(std::format("{:02x}", h & 0xFF));
    }

    return hash_str;
}

} // namespace day10
