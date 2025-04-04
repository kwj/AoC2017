module;

#include <algorithm>
#include <istream>
#include <ranges>
#include <string>
#include <tuple>
#include <vector>

// --------
export module day04;

export namespace day04 {

using Passphrase = std::vector<std::string>;

std::tuple<long, long> solve(std::istream &is);
std::vector<Passphrase> parse(std::istream &is);
long part1(std::vector<Passphrase> const &ps);
long part2(std::vector<Passphrase> const &ps);

} // namespace day04

// --------
module :private;

namespace day04 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

std::vector<Passphrase>
parse(std::istream &is) {
    std::vector<Passphrase> result;

    for (std::string line; std::getline(is, line);) {
        result.push_back(
            line | std::views::split(' ') | std::ranges::to<Passphrase>()
        );
    }

    return result;
}

bool
isValid(Passphrase words) {
    std::ranges::sort(words);
    for (auto i = 0uz; i < words.size() - 1; ++i) {
        if (words[i] == words[i + 1]) {
            return false;
        }
    }

    return true;
}

long
part1(std::vector<Passphrase> const &ps) {
    return std::ranges::count_if(ps, isValid);
}

long
part2(std::vector<Passphrase> const &ps) {
    long result {0};

    for (auto passphrase : ps) {
        for (auto &word : passphrase) {
            std::ranges::sort(word);
        }
        if (isValid(passphrase)) {
            ++result;
        }
    }

    return result;
}

} // namespace day04
