module;

#include <algorithm>
#include <format>
#include <functional>
#include <istream>
#include <numeric>
#include <sstream>
#include <string>
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

}  // namespace day14

// --------
module :private;

namespace day14 {

constexpr long KNOTS_LEN{256};
constexpr size_t EDGE_LEN{128};

std::tuple<long, long> solve(std::istream &is) {
    auto result = parse(is);

    return {part1(result), part2(result)};
}

std::vector<long> knotHash(std::vector<long> lengths, long cnt) {
    auto knots = std::vector<long>(KNOTS_LEN);
    std::iota(knots.begin(), knots.end(), 0);

    long skip{0};
    long origin{0};
    for (auto round = 0; round < cnt; ++round) {
        for (auto const len : lengths) {
            std::reverse(knots.begin(), knots.begin() + len);

            auto offset = (len + skip) % KNOTS_LEN;
            std::rotate(knots.begin(), knots.begin() + offset, knots.end());
            origin += offset;
            ++skip;
        }
    }

    std::rotate(knots.begin(), knots.end() - (origin % KNOTS_LEN), knots.end());

    return knots;
}

std::vector<long> makeGrid(std::string_view prefix) {
    std::vector<long> grid;
    std::vector<long> lengths;
    std::vector<long> const tail{17, 31, 73, 47, 23};

    for (size_t k = 0; k < EDGE_LEN; ++k) {
        lengths.clear();
        auto s = std::format("{}-{}", prefix, k);
        std::istringstream iss(s);
        char ch;

        while (iss.get(ch)) {
            lengths.push_back(static_cast<long>(ch));
        }
        lengths.append_range(tail);

        auto row = knotHash(lengths, 64);
        for (auto i = 0; i < KNOTS_LEN; i += 16) {
            auto h = std::ranges::fold_left(row.begin() + i, row.begin() + i + 16, 0, std::bit_xor<long>());
            for (long mask = 0b10000000; mask > 0; mask >>= 1) {
                if ((h & mask) != 0) {
                    grid.push_back(1);
                } else {
                    grid.push_back(0);
                }
            }
        }
    }

    return grid;
}

std::vector<long> parse(std::istream &is) {
    auto prefix = util::getLine(is);

    return makeGrid(prefix);
}

long part1(std::vector<long> const &grid) {
    return std::count_if(grid.begin(), grid.end(), [](auto x){ return x != 0; });
}

std::vector<size_t> getNbrs(size_t idx) {
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

void checkSpacesInRegion(std::vector<long> &squares, size_t idx) {
    for (auto const &nbrs_idx : getNbrs(idx)) {
        if (squares[nbrs_idx] != 0) {
            squares[nbrs_idx] = 0;
            checkSpacesInRegion(squares, nbrs_idx);
        }
    }

    return;
}

long part2(std::vector<long> const &grid) {
    // 0: free space, or checked used space
    // 1: unchecked used space
    std::vector<long> squares{grid};

    long n_regions{0};
    for (size_t idx{0}; idx < squares.size(); ++idx) {
        if (squares[idx] != 0) {
            squares[idx] = 0;
            checkSpacesInRegion(squares, idx);
            ++n_regions;
        }
    }

    return n_regions;
}

} // namespace day14
