/*
[Part 2]
There is no solution that can solve the general input. For example,
the answer cannot be fixed for the following input data. (b = 2 or c = 1)

  a (2) -> b, c
  b (1)
  c (2)

Nor can the following input data be used to fix the answer. (b = 3 or c = 1 or f = 4)

  a (2) -> b, c
  b (2) -> f
  c (2) -> d, e
  d (2)
  e (2)
  f (3)

My solution therefore assumes that there is one and only answer for a given input data.
*/

module;

#include <algorithm>
#include <istream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

struct Disc {
    std::string name;
    size_t id;
    size_t parent;
    long weight;
    std::vector<size_t> children;
    std::vector<long> sub_tower_weight; // each child tower's total weight
};

// --------
export module day07;

export namespace day07 {

std::tuple<std::string, long> solve(std::istream &is);
std::vector<Disc> parse(std::istream &is);
std::string part1(std::vector<Disc> const &discs);
long part2(std::vector<Disc> &discs);

}  // namespace day07

// --------
module :private;

namespace day07 {

std::tuple<std::string, long> solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

std::vector<Disc> parse(std::istream &is) {
    using namespace std::literals;

    std::smatch m;
    std::regex re(R"((\w+) \((\d+)\)( -> ((\w+, )*\w+))?)");
    //                                   ^^^^^^^^^^^^^^ m[4]

    std::vector<Disc> discs{{"dummy", 0, 0, 0, {}, {}}};
    std::unordered_map<std::string, size_t> idMap;
    std::unordered_map<size_t, std::vector<std::string>> childMap;
    size_t idx{1};

    for (std::string line; std::getline(is, line);) {
        if (std::regex_match(line, m, re)) {
            auto name = m[1].str();
            auto weight = std::stol(m[2].str());

            discs.push_back({name, idx, 0, weight, {}, {}});
            idMap[name] = idx;

            if (m.length(4) > 0) {
                std::vector<std::string> children;
                for (auto const word : std::views::split(m[4].str(), ", "sv)) {
                    children.push_back(std::string(std::string_view(word)));
                }
                childMap[idx] = children;
            }

            ++idx;
        }
    }

    for (auto const &[parent_id, children] : childMap) {
        std::for_each(children.begin(), children.end(), [&](std::string const &name){
            auto child_id = idMap[name];
            discs[parent_id].children.push_back(child_id);
            discs[parent_id].sub_tower_weight.push_back(0); // tentative value. it will be updated by calcWeight().
            discs[child_id].parent = parent_id;
        });
    }

    return discs;
}

size_t findRootId(std::vector<Disc> const &discs) {
    size_t idx{1}; // ignore discs[0] dummy element

    while (discs[idx].parent != 0) {
        idx = discs[idx].parent;
    }

    return idx;
}

std::string part1(std::vector<Disc> const &discs) {
    return discs[findRootId(discs)].name;
}

long calcWeight(std::vector<Disc> &discs, size_t id) {
    Disc &self = discs[id];

    // P2164R9 (views::enumerate) is not yet supported in libc++ 19.
    for (size_t idx = 0; idx < self.children.size(); ++idx) {
        self.sub_tower_weight[idx] = calcWeight(discs, self.children[idx]);
    }

    return self.weight + std::accumulate(self.sub_tower_weight.begin(), self.sub_tower_weight.end(), 0);
}

bool isBalanced(Disc const &disc) {
    return std::all_of(disc.sub_tower_weight.begin(), disc.sub_tower_weight.end(), [&](long w) {
        return disc.sub_tower_weight[0] == w;
    });
}

long findDelta(std::vector<Disc> const &discs, size_t root_id) {
    auto &self = discs[root_id];

    // assume that self.children.size() >= 2 because the root disc has two or more child discs.
    switch (self.children.size()) {
        case 2:
            if (isBalanced(discs[self.children[0]])) {
                return self.sub_tower_weight[0] - self.sub_tower_weight[1];
            } else {
                return self.sub_tower_weight[1] - self.sub_tower_weight[0];
            }
        default:
            std::vector<long> work{self.sub_tower_weight};
            std::sort(work.begin(), work.end());
            if (work[0] == work[1]) {
                return work[0] - work.back();
            } else {
                return work.back() - work[0];
            }
    }
}

std::tuple<size_t, long> findBadDisc(std::vector<Disc> const &discs, size_t id, long delta) {
    auto &self = discs[id];

    // this leaf disc is the bad disc.
    if (self.children.size() == 0) {
        return std::make_tuple(id, self.weight + delta);
    }

    // if each child disc has same sub-total weight, the disc `discs[id]` is the bad disc.
    if (isBalanced(discs[id])) {
        return std::make_tuple(id, self.weight + delta);
    }

    // otherwise (assume that self.children.size() >= 2)
    if (self.children.size() == 2) {
        if (self.sub_tower_weight[0] + delta == self.sub_tower_weight[1]) {
            return findBadDisc(discs, self.children[0], delta);
        } else {
            return findBadDisc(discs, self.children[1], delta);
        }
    } else {
        auto key = self.sub_tower_weight[0];

        if (std::all_of(self.sub_tower_weight.begin() + 1, self.sub_tower_weight.end(), [&](long w) { return key != w; })) {
            return findBadDisc(discs, self.children[0], delta);
        } else {
            auto it = std::find_if(self.sub_tower_weight.begin() + 1, self.sub_tower_weight.end(), [&](long w) { return key != w; });
            return findBadDisc(discs, self.children[static_cast<size_t>(it - self.sub_tower_weight.begin())], delta);
        }
    }
}

long part2(std::vector<Disc> &discs) {
    auto root = findRootId(discs);

    calcWeight(discs, root);
    auto bad_disc = findBadDisc(discs, root, findDelta(discs, root));

    return std::get<1>(bad_disc);
}

} // namespace day07
