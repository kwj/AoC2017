module;

#include <algorithm>
#include <istream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <tuple>
#include <vector>

import util;

// --------
export module day13;

export namespace day13 {

std::tuple<long, long> solve(std::istream &is);
std::map<long, std::set<long>> parse(std::istream &is);
long part1(std::map<long, std::set<long>> const &tbl);
long part2(std::map<long, std::set<long>> const &tbl);

} // namespace day13

// --------
module :private;

namespace day13 {

std::tuple<long, long>
solve(std::istream &is) {
    auto result = parse(is);

    return {part1(result), part2(result)};
}

std::map<long, std::set<long>>
parse(std::istream &is) {
    // Note the use an ordered map to solve part 2.
    // key: range, value: a set of depths
    std::map<long, std::set<long>> result;

    for (std::string line; std::getline(is, line);) {
        std::vector<long> nums = util::getNumbers(line);
        result[nums[1]].insert(nums[0]);
    }

    return result;
}

bool
isCaught(long period, long depth, long delay = 0) {
    return (depth + delay) % period == 0;
}

long
part1(std::map<long, std::set<long>> const &tbl) {
    long severity {0};
    for (auto const &[r, depths] : tbl) {
        // period to scan the top of a layer which range is `r`.
        auto period = 2 * (r - 1);

        for (auto const &d : depths) {
            if (isCaught(period, d)) {
                severity += d * r;
            }
        }
    }

    return severity;
}

long
part2(std::map<long, std::set<long>> const &tbl) {
    long crnt_lcm {1};
    std::vector<long> delays {1};
    std::vector<long> next_delays;

    for (auto const &[r, depths] : tbl) {
        // period to scan the top of a layer whose range is `r`.
        auto period = 2 * (r - 1);

        auto next_lcm = std::lcm(crnt_lcm, period);
        next_delays.clear();
        for (auto const &dly : delays) {
            for (auto gap = 0l; gap < next_lcm; gap += crnt_lcm) {
                auto next_dly = dly + gap;

                // assume that initial delay is next_dly, check if the packet doesn't
                // caught by each layer whose period is 2(r - 1).
                if (std::ranges::all_of(depths, [&](auto const &depth) {
                        return !isCaught(period, depth, next_dly);
                    })) {
                    next_delays.push_back(next_dly);
                }
            }
        }

        std::ranges::swap(delays, next_delays);
        crnt_lcm = next_lcm;
    }

    return *std::ranges::min_element(delays);
}

} // namespace day13
