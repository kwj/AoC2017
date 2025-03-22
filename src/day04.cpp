module;

#include <algorithm>
#include <ranges>
#include <istream>
#include <string>
#include <tuple>
#include <vector>

// --------
export module day04;

export namespace day04 {

std::tuple<long, long> solve(std::istream &is);
std::vector<std::vector<std::string>> parse(std::istream &is);
long part1(std::vector<std::vector<std::string>> &vvs);
long part2(std::vector<std::vector<std::string>> &vvs);

} // namespace day04

// --------
module :private;

namespace day04 {

std::tuple<long, long> solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

std::vector<std::vector<std::string>> parse(std::istream &is) {
    std::vector<std::vector<std::string>> result;

    for (std::string line; std::getline(is, line);) {
        result.push_back(line | std::views::split(' ') | std::ranges::to<std::vector<std::string>>());
    }

    return result;
}

bool isValid(std::vector<std::string> const &vs) {
    std::vector<std::string> words{vs};
    std::ranges::sort(words);

    for (std::size_t i = 0; i < words.size() - 1; ++i) {
        if (words[i] == words[i + 1]) {
            return false;
        }
    }

    return true;
}

long part1(std::vector<std::vector<std::string>> &vvs) {
    return static_cast<long>(std::count_if(vvs.begin(), vvs.end(), isValid));
}

long part2(std::vector<std::vector<std::string>> &vvs) {
    std::vector<std::vector<std::string>> tmp_vvs{vvs};
    for (auto &vs : tmp_vvs) {
        for (auto &word : vs) {
            std::ranges::sort(word);
        }
    }

    return static_cast<long>(std::count_if(tmp_vvs.begin(), tmp_vvs.end(), isValid));
}

} // namespace day04
