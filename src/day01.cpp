module;

#include <algorithm>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>

// --------
export module day01;

export namespace day01 {

std::tuple<long, long> solve(std::stringstream &ss);
std::string parse(std::stringstream &ss);
long part1(std::string const &s);
long part2(std::string const &s);

} // namespace day01

// --------
module :private;

namespace day01 {

std::tuple<long, long> solve(std::stringstream &ss) {
    auto input_data = parse(ss);

    return {part1(input_data), part2(input_data)};
}

std::string parse(std::stringstream &ss) {
    std::string line;
    std::getline(ss, line);

    return line;
}

long captcha(std::string const &s1, long offset) {
    std::string s2{s1};
    std::rotate(s2.begin(), s2.begin() + offset, s2.end());

    long result{0};
    for (auto tpl : std::views::zip(s1, s2)) {
        if (std::get<0>(tpl) == std::get<1>(tpl)) {
            result += std::get<0>(tpl) - '0';
        }
    }

    return result;
}

long part1(std::string const &s) {
    return captcha(s, 1);
}

long part2(std::string const &s) {
    return captcha(s, s.size() / 2);
}

}  // module day01

