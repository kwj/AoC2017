module;

#include <algorithm>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

import util;

// --------
export module day02;

export namespace day02 {

std::tuple<long, long> solve(std::stringstream &ss);
std::vector<std::vector<long>> parse(std::stringstream &ss);
long part1(std::vector<std::vector<long>> const &vvs);
long part2(std::vector<std::vector<long>> const &vvs);

}  // namespace day02

// --------
module :private;

namespace day02 {

std::tuple<long, long> solve(std::stringstream &ss) {
    auto input_data = parse(ss);

    return {part1(input_data), part2(input_data)};
}

std::vector<std::vector<long>> parse(std::stringstream &ss) {
    std::vector<std::vector<long>> result;

    for (std::string line; std::getline(ss, line);) {
        auto nums = util::getNumbers(line);
        std::ranges::sort(nums, std::ranges::greater());
        result.push_back(nums);
    }

    return result;
}

long part1(std::vector<std::vector<long>> const &vvs) {
    int result{0};

    // Note: `vs` is a list of int in descending order.
    for (auto const &vs : vvs) {
        result += vs.front() - vs.back();
    }

    return result;
}

// Note: `vs` is a list of int in descending order.
long findQuotient(std::vector<long> const &vs) {
    auto n_elm{vs.size()};
    for (std::size_t i = 0; i < n_elm - 1; ++i) {
        for (std::size_t j = i + 1; j < n_elm; ++j) {
            if (vs[i] % vs[j] == 0) {
                return vs[i] / vs[j];
            }
        }
    }

    return 0;
}

long part2(std::vector<std::vector<long>> const &vvs) {
    long result{0};

    for (auto const &vs : vvs) {
        result += findQuotient(vs);
    }

    return result;
}

} // namespace day02
