/*
Assume that program IDs are arranged in such a way that each successive ID number is
greater than one before it in the given input data, and they are started from 0.
*/

module;

#include <istream>
#include <ranges>
#include <string>
#include <tuple>
#include <vector>

import util;

// --------
export module day12;

export namespace day12 {

std::tuple<long, long> solve(std::istream &is);
std::vector<long> parse(std::istream &is);
long part1(std::vector<long> const &grps);
long part2(std::vector<long> const &grps);

} // namespace day12

// --------
module :private;

namespace day12 {

std::tuple<long, long>
solve(std::istream &is) {
    auto result = parse(is);

    return {part1(result), part2(result)};
}

long
countProgramsInGroup(
    std::vector<std::vector<size_t>> const &connections,
    std::vector<bool> &id_tbl,
    size_t id
) {
    long cnt {1};
    for (auto const &nbr_id : connections[id]) {
        if (!id_tbl[nbr_id]) {
            id_tbl[nbr_id] = true;
            cnt += countProgramsInGroup(connections, id_tbl, nbr_id);
        }
    }

    return cnt;
}

std::vector<long>
parse(std::istream &is) {
    std::vector<std::vector<size_t>> connections;
    std::string line;
    while (std::getline(is, line)) {
        std::vector<long> nums = util::getNumbers(line);

        // ignore program ID (The reason for this is the comment at the beginning of this file)
        connections.push_back(
            std::ranges::drop_view {nums, 1} |
            std::ranges::to<std::vector<size_t>>()
        );
    }

    // Each element is a number of programs in a group. The first element result[0]
    // indicates the group containing the program 0, however, the rest elements are
    // unknown. It cuts corners.
    std::vector<long> result;

    // All elements are initialized to default value `false`.
    // It means that the ID does not checked yet.
    std::vector<bool> id_tbl(connections.size());

    for (auto id = 0uz; id < connections.size(); ++id) {
        if (!id_tbl[id]) {
            id_tbl[id] = true;
            result.push_back(countProgramsInGroup(connections, id_tbl, id));
        }
    }

    return result;
}

long
part1(std::vector<long> const &grps) {
    return grps[0];
}

long
part2(std::vector<long> const &grps) {
    return static_cast<long>(grps.size());
}

} // namespace day12
