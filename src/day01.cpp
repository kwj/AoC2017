module;

#include <algorithm>
#include <istream>
#include <ranges>
#include <string>
#include <tuple>

import util;

// --------
export module day01;

export namespace day01 {

std::tuple<long, long> solve(std::istream &is);
std::string parse(std::istream &is);
long part1(std::string const &s);
long part2(std::string const &s);

} // namespace day01

// --------
module :private;

namespace day01 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

std::string
parse(std::istream &is) {
    return util::getLine(is);
}

long
captcha(std::string const &s1, long offset) {
    std::string s2 {s1};
    std::rotate(s2.begin(), s2.begin() + offset, s2.end());

    long result {0};
    for (auto const [x, y] : std::views::zip(s1, s2)) {
        if (x == y) {
            result += x - '0';
        }
    }

    return result;
}

long
part1(std::string const &s) {
    return captcha(s, 1);
}

long
part2(std::string const &s) {
    return captcha(s, s.size() / 2);
}

} // namespace day01
