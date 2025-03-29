/*
[Part 2]
There is no solution that can solve the general input. For example,
the answer cannot be fixed for the following input data. (b = 2 or c = 1)

  a (2) -> b, c
  b (1)
  c (2)

Nor can the following input data be used to fix the answer.
(b = 3 or c = 1 or f = 4)

  a (2) -> b, c
  b (2) -> f
  c (2) -> d, e
  d (2)
  e (2)
  f (3)

My solution therefore assumes that there is one and only answer for a given
input data.
*/

module;

#include <algorithm>
#include <cstddef>
#include <functional>
#include <istream>
#include <ranges>
#include <regex>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

// --------
export module day07;

export namespace day07 {

struct Disc {
    std::string name;
    size_t id;
    size_t parent;
    long weight;
    std::vector<std::pair<size_t, long>> children; // { id, sub tower's weight }
};

std::tuple<std::string, long> solve(std::istream &is);
std::vector<Disc> parse(std::istream &is);
std::string part1(std::vector<Disc> const &discs);
long part2(std::vector<Disc> const &discs);

} // namespace day07

// --------
module :private;

namespace day07 {

std::tuple<std::string, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

std::vector<Disc>
parse(std::istream &is) {
    using namespace std::literals;

    std::smatch m;
    std::regex const re(R"((\w+) \((\d+)\)( -> ((\w+, )*\w+))?)");
    //                                         ^^^^^^^^^^^^^^ m[4]

    std::vector<Disc> discs {
        {.name = "sentinel", .id = 0, .parent = 0, .weight = 0, .children = {}}
    };
    std::unordered_map<std::string, size_t> idMap;
    std::unordered_map<size_t, std::vector<std::string>> childMap;

    size_t idx {1};
    for (std::string line; std::getline(is, line);) {
        if (std::regex_match(line, m, re)) {
            auto name = m[1].str();
            auto weight = std::stol(m[2].str());

            discs.emplace_back(
                name, idx, 0, weight, std::vector<std::pair<size_t, long>> {}
            );
            idMap[name] = idx;

            if (m.length(4) > 0) {
                std::vector<std::string> names;
                for (auto const &subr : std::views::split(m[4].str(), ", "sv)) {
                    names.emplace_back(subr.begin(), subr.end());
                }
                childMap[idx] = names;
            }

            ++idx;
        }
    }

    for (auto const &[parent_id, children] : childMap) {
        std::ranges::for_each(children, [&](std::string const &name) {
            auto child_id = idMap[name];
            // NOLINTNEXTLINE (clang-analyzer-core.NullDereference)
            discs[parent_id].children.emplace_back(child_id, 0);
            discs[child_id].parent = parent_id;
        });
    }

    return discs;
}

size_t
findRootId(std::vector<Disc> const &discs) {
    size_t idx {1}; // skip discs[0] since it is the sentinel element

    while (discs[idx].parent != 0) {
        idx = discs[idx].parent;
    }

    return idx;
}

std::string
part1(std::vector<Disc> const &discs) {
    return discs[findRootId(discs)].name;
}

long
calcWeight(std::vector<Disc> &discs, size_t id) {
    Disc &self = discs[id];

    for (auto &child : self.children) {
        child.second = calcWeight(discs, child.first);
    }

    return self.weight +
           std::ranges::fold_left(
               self.children | std::views::values, 0, std::plus<long> {}
           );
}

bool
isBalanced(Disc const &disc) {
    return std::ranges::all_of(disc.children | std::views::values, [&](long w) {
        return disc.children[0].second == w;
    });
}

long
findDelta(std::vector<Disc> const &discs, size_t root_id) {
    auto &self = discs[root_id];

    // assume that self.children.size() >= 2
    // since the root disc has two or more child discs.
    if (self.children.size() == 2) {
        if (isBalanced(discs[self.children[0].first])) {
            return self.children[0].second - self.children[1].second;
        } else {
            return self.children[1].second - self.children[0].second;
        }
    } else {
        auto work {self.children};
        std::ranges::sort(work, [](auto &a, auto &b) {
            return a.second < b.second;
        });
        if (work[0].second == work[1].second) {
            return work[0].second - work.back().second;
        } else {
            return work.back().second - work[0].second;
        }
    }
}

std::pair<size_t, long>
findBadDisc(std::vector<Disc> const &discs, size_t id, long delta) {
    auto &self = discs[id];

    // this leaf disc is the bad disc.
    if (self.children.size() == 0) {
        return std::make_pair(id, self.weight + delta);
    }

    // if each child disc has same sub-total weight, the disc `discs[id]` is
    // the bad disc.
    if (isBalanced(discs[id])) {
        return std::make_pair(id, self.weight + delta);
    }

    // otherwise (assume that self.children.size() >= 2)
    if (self.children.size() == 2) {
        if (self.children[0].second + delta == self.children[1].second) {
            return findBadDisc(discs, self.children[0].first, delta);
        } else {
            return findBadDisc(discs, self.children[1].first, delta);
        }
    } else {
        auto key = self.children[0].second;

        if (std::ranges::all_of(
                self.children | std::views::drop(1),
                [&](auto &p) { return key != p.second; }
            )) {
            return findBadDisc(discs, self.children[0].first, delta);
        } else {
            auto it = std::ranges::find_if(
                self.children | std::views::drop(1),
                [&](auto &p) { return key != p.second; }
            );

            return findBadDisc(discs, it->first, delta);
        }
    }
}

long
part2(std::vector<Disc> const &discs) {
    auto tmp_discs {discs};
    auto root = findRootId(tmp_discs);

    calcWeight(tmp_discs, root);

    // { disk_id, corrected weight }
    return findBadDisc(tmp_discs, root, findDelta(tmp_discs, root)).second;
}

} // namespace day07
