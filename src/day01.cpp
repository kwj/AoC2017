module;

#include <algorithm>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>

// --------
export module day01;

export namespace day01 {

std::tuple<int, int> solve(std::stringstream &ss);
std::string parse(std::stringstream &ss);
int part1(std::string const &s);
int part2(std::string const &s);

} // namespace day01

// --------
module :private;

namespace day01 {

std::tuple<int, int> solve(std::stringstream &ss) {
    auto input_data = parse(ss);

    return {part1(input_data), part2(input_data)};
}

std::string parse(std::stringstream &ss) {
    std::string line;
    std::getline(ss, line);

    return line;
}

int captcha(std::string const &s1, int offset) {
    std::string s2{s1};
    std::rotate(s2.begin(), s2.begin() + offset, s2.end());

    int result{0};
    for (auto tpl : std::views::zip(s1, s2)) {
        if (std::get<0>(tpl) == std::get<1>(tpl)) {
            result += std::get<0>(tpl) - '0';
        }
    }

    return result;
}

int part1(std::string const &s) {
    return captcha(s, 1);
}

int part2(std::string const &s) {
    return captcha(s, s.size() / 2);
}

}  // module day01

