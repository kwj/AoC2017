module;

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <format>
#include <functional>
#include <istream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

import util;

// --------
export module day14;

export namespace day14 {

std::tuple<long, long> solve(std::istream &is);
std::vector<long> parse(std::istream &is);
long part1(std::vector<long> const &grid);
long part2(std::vector<long> const &grid);

} // namespace day14

// --------
module :private;

namespace day14 {

constexpr unsigned long BLOCK_LEN {16};
constexpr unsigned long KNOTS_LEN {BLOCK_LEN * 16};
constexpr size_t EDGE_LEN {128};

std::tuple<long, long>
solve(std::istream &is) {
    auto result = parse(is);

    return {part1(result), part2(result)};
}

std::array<long, KNOTS_LEN>
knotHash(std::vector<unsigned long> lengths, long round) {
    std::array<long, KNOTS_LEN> knots;

// P2440R1 (ranges::iota) is not yet supported in libc++ 19.
#if __cpp_lib_ranges_iota >= 202202L
    std::ranges::iota(knots, 0);
#else
    std::iota(knots.begin(), knots.end(), 0);
#endif

    unsigned long skip {0};
    unsigned long pos {0};
    while (round-- > 0) {
        for (auto len : lengths) {
            // reverse the order by swapping elements
            auto x = pos;
            auto y = (pos + len - 1) % KNOTS_LEN;
            for (auto cnt = 0ul; cnt < len / 2; ++cnt) {
                std::ranges::swap(knots[x], knots[y]);
                x = (x + 1) % KNOTS_LEN;
                y = y == 0 ? KNOTS_LEN - 1 : y - 1;
            }

            // move position
            pos = (pos + len + skip) % KNOTS_LEN;
            ++skip;
        }
    }

    return knots;
}

std::vector<long>
makeGrid(std::string_view prefix) {
    // 0: free square
    // not 0: used square
    std::vector<long> grid;

    std::vector<unsigned long> lengths;
    std::vector<unsigned long> const tail {17, 31, 73, 47, 23};

    for (auto k = 0uz; k < EDGE_LEN; ++k) {
        lengths.clear();
        auto s = std::format("{}-{}", prefix, k);
        std::istringstream iss(s);
        char ch;

        while (iss.get(ch)) {
            lengths.push_back(static_cast<unsigned long>(ch));
        }

// P1206R7 (range to container conversion) is not yet fully supported in GCC 14.
#if __cpp_lib_containers_ranges >= 202202L
        lengths.append_range(tail);
#else
        lengths.insert(lengths.end(), tail.cbegin(), tail.cend());
#endif

        auto hash = knotHash(lengths, 64);
        for (auto it = hash.cbegin(); it <= hash.cend() - BLOCK_LEN;
             std::ranges::advance(it, BLOCK_LEN)) {
            auto h = std::ranges::fold_left(
                std::ranges::subrange(it, it + BLOCK_LEN),
                0,
                std::bit_xor<long> {}
            );
            for (long mask = 0b10000000; mask > 0; mask >>= 1) {
                grid.push_back(h & mask);
            }
        }
    }

    return grid;
}

std::vector<long>
parse(std::istream &is) {
    auto prefix = util::getLine(is);

    return makeGrid(prefix);
}

long
part1(std::vector<long> const &grid) {
    return std::ranges::count_if(grid, [](auto x) { return x != 0; });
}

std::vector<size_t>
getNbrs(size_t idx) {
    std::vector<size_t> nbrs;
    auto q = idx / EDGE_LEN;
    auto r = idx % EDGE_LEN;

    if (q > 0) {
        // upper side
        nbrs.push_back(idx - EDGE_LEN);
    }
    if (q < EDGE_LEN - 1) {
        // bottom side
        nbrs.push_back(idx + EDGE_LEN);
    }
    if (r > 0) {
        // left side
        nbrs.push_back(idx - 1);
    }
    if (r < EDGE_LEN - 1) {
        // right side
        nbrs.push_back(idx + 1);
    }

    return nbrs;
}

void
checkSpacesInRegion(std::vector<long> &squares, size_t idx) {
    for (auto nbrs_idx : getNbrs(idx)) {
        if (squares[nbrs_idx] != 0) {
            squares[nbrs_idx] = 0;
            checkSpacesInRegion(squares, nbrs_idx);
        }
    }

    return;
}

long
part2(std::vector<long> const &grid) {
    // 0: free space, or checked used space
    // not 0: unchecked used space
    std::vector<long> squares {grid};

    long n_regions {0};
    for (auto idx = 0uz; idx < squares.size(); ++idx) {
        if (squares[idx] != 0) {
            squares[idx] = 0;
            checkSpacesInRegion(squares, idx);
            ++n_regions;
        }
    }

    return n_regions;
}

} // namespace day14
