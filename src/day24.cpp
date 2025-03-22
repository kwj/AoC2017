module;

#include <algorithm>
#include <istream>
#include <string>
#include <tuple>
#include <vector>

import util;

// --------
export module day24;

export namespace day24 {

struct Component {
    bool used;
    long port1;
    long port2;
    long weight;
};

std::tuple<long, long> solve(std::istream &is);
std::pair<long, long> parse(std::istream &is);
long part1(std::pair<long, long> result);
long part2(std::pair<long, long> result);

} // namespace day24

// --------
module :private;

namespace day24 {

std::tuple<long, long> solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

template <typename Fn>
void dfs(std::vector<Component> &cmpnts, long target, long total_weight, long depth, Fn f) {
    for (auto &[used, p1, p2, w] : cmpnts) {
        if (used) {
            continue;
        }

        if (target == p1 || target == p2) {
            used = true;
            dfs(cmpnts, target ^ p1 ^ p2, total_weight + w, depth + 1, f);
            used = false;
        }
    }
    f(total_weight, depth);

    return;
}

std::pair<long, long> parse(std::istream &is) {
    std::vector<Component> cmpnts;
    for (std::string line; std::getline(is, line);) {
        auto nums = util::getNumbers(line);
        cmpnts.push_back({false, nums[0], nums[1], nums[0] + nums[1]});
    }

    long max_strength{0};
    long max_strength_when_longest{0};
    long max_length{0};
    auto f = [&](long w, long l) {
        max_strength = std::max(max_strength, w);
        if (l >= max_length) {
            max_strength_when_longest = std::max(max_strength_when_longest, w);
            max_length = l;
        }
    };
    dfs(cmpnts, 0, 0, 0, f);

    return {max_strength, max_strength_when_longest};
}

long part1(std::pair<long, long> result) {
    return result.first;
}

long part2(std::pair<long, long> result) {
    return result.second;
}

} // namespace day24
