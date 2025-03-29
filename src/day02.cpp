module;

#include <algorithm>
#include <istream>
#include <string>
#include <tuple>
#include <vector>

import util;

// --------
export module day02;

export namespace day02 {

std::tuple<long, long> solve(std::istream &is);
std::vector<std::vector<long>> parse(std::istream &is);
long part1(std::vector<std::vector<long>> const &vvs);
long part2(std::vector<std::vector<long>> const &vvs);

} // namespace day02

// --------
module :private;

namespace day02 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

std::vector<std::vector<long>>
parse(std::istream &is) {
    std::vector<std::vector<long>> result;

    for (std::string line; std::getline(is, line);) {
        auto nums = util::getNumbers(line);
        std::ranges::sort(nums, std::ranges::greater());
        result.push_back(nums);
    }

    return result;
}

long
part1(std::vector<std::vector<long>> const &vvs) {
    long result {0};

    // Note: `vs` is a list of long in descending order.
    for (auto const &vs : vvs) {
        result += vs.front() - vs.back();
    }

    return result;
}

// Note: `vs` is a list of int in descending order.
long
findQuotient(std::vector<long> const &vs) {
    auto const n_elm {vs.size()};
    for (auto i = 0uz; i < n_elm - 1; ++i) {
        for (auto j = i + 1; j < n_elm; ++j) {
            if (vs[i] % vs[j] == 0) {
                return vs[i] / vs[j];
            }
        }
    }

    return 0;
}

long
part2(std::vector<std::vector<long>> const &vvs) {
    long result {0};

    for (auto const &vs : vvs) {
        result += findQuotient(vs);
    }

    return result;
}

} // namespace day02
