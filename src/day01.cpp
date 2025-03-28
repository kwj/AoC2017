module;

#include <algorithm>
#include <istream>
#include <ranges>
#include <string>
#include <string_view>
#include <tuple>

import util;

// --------
export module day01;

export namespace day01 {

std::tuple<long, long> solve(std::istream &is);
std::string parse(std::istream &is);
long part1(std::string_view s);
long part2(std::string_view s);

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
captcha(std::string_view s, size_t offset) {
    long result {0};
    for (auto x = 0uz, y = (x + offset) % s.size(); x < s.size(); ++x, ++y) {
        if (y >= s.size()) {
            y -= s.size();
        }
        if (s[x] == s[y]) {
            result += s[x] - '0';
        }
    }

    return result;
}

long
part1(std::string_view s) {
    return captcha(s, 1);
}

long
part2(std::string_view s) {
    return captcha(s, s.size() / 2);
}

} // namespace day01
