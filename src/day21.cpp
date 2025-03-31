/*
Example rules
-------------
#./.. => .#./#../##.
##/#. => #../.##/#..
##/.. => #../#.#/..#
.#/#. => .##/#.#/#..
##/## => #.#/.#./#.#
.#./..#/### => ###./.###/..#./##.#

[step 0]
  .#.
  ..#
  ###

[step 1]
  ##|#.
  .#|##
  --+--
  ..|#.
  ##|.#

[step 2]
  #.|.#|..
  .#|#.|##
  --+--+--
  #.|.#|..
  #.|..|##
  --+--+--
  #.|##|.#
  ..|##|..

[step 3]
  .##|.##|#..
  #.#|#.#|#.#
  #..|#..|..#
  ---+---+---
  #..|.#.|#..
  #.#|#..|#.#
  ..#|##.|..#
  ---+---+---
  .#.|#.#|.#.
  #..|.#.|#..
  ##.|#.#|##.

nine 3x3 grids:
  .##
  #.# x 2
  #..

  #..
  #.# x 3
  ..#

  .#.
  #.. x 3
  ##.

  #.#
  .#. x 1
  #.#

[Part 1 and 2]
Three iterations from a 3x3 grid will transition to a state with nine 3x3 grids.
I therefore decided to create a transition table based on three iterations and
used recursion to find the answer.

std::map<size_t, TransGrid> trans_tbl;
  Transition table with a 3x3 grids as a key. A key is obtained as follows.

    .#.
    ..# --> {0, 1, 0, 0, 0, 1, 1, 1, 1} --> 0b010001111 -> 143
    ###

  Only seven entries are needed at most.
    number of '2x2 to 3x3' patterns(6) + start grid(1) = 7

struct TransGrid {
    std::array<long, 4> pop_count;
    std::vector<std::pair<size_t, long>> next_grids;
};

  pop_count:
    Number of lit pixels after 0 to 3 iterations
  next_grids:
    Nine 3x3 grids after three iterations.

In the above example, it would look like this:

  trans_tbl[143] = {
      .pop_count = {5, 10, 17, 39},
      .next_grids = {{236, 2}, {297, 3}, {166, 3}, {341, 1}}
  };
*/

module;

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <istream>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

// --------
export module day21;

export namespace day21 {

struct [[nodiscard]] TransGrid {
    std::array<long, 4> pop_count;
    std::vector<std::pair<size_t, long>> next_grids;
};

using TransTbl = std::map<size_t, TransGrid>;

std::tuple<long, long> solve(std::istream &is);
TransTbl parse(std::istream &is);
long part1(TransTbl const &trans_tbl);
long part2(TransTbl const &trans_tbl);

} // namespace day21

// --------
module :private;

namespace day21 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

//  the start grid (glider)
//    .#.
//    ..# --> {0, 1, 0, 0, 0, 1, 1, 1, 1}
//    ###
std::array<size_t, 9> const start_grid = {0, 1, 0, 0, 0, 1, 1, 1, 1};

struct [[nodiscard]] Map_2x2to3x3 {
    std::array<size_t, 9> bits; // 3x3 grid after transition
    long pop_count;             // number of lit pixels in 2x2 grid
};

struct [[nodiscard]] Map_3x3to4x4 {
    std::array<size_t, 16> bits; // 4x4 grid after transition
    long pop_count;              // number of lit pixels in 3x3 grid
};

template <typename T>
size_t
bitsToId(T const &bit_seq) {
    size_t id {0};

    for (auto const b : bit_seq) {
        id = (id << 1) | b;
    }

    return id;
}

template <typename T>
std::set<size_t>
getVariants_2x2(T const &bit_seq) {
    std::set<size_t> ids;
    auto w {bit_seq};

    for (auto i = 0; i < 8; ++i) {
        ids.insert(bitsToId(w));

        // rotate
        //   0 1  --> 2 0
        //   2 3      3 1
        w = {w[2], w[0], w[3], w[1]};

        // flip
        //   0 1  --> 2 3
        //   2 3      0 1
        if (i == 3) {
            w = {w[2], w[3], w[0], w[1]};
        }
    }

    return ids;
}

// Update the 2x2 to 3x3 mapping table and return a 3x3 grid's ID which is used as a key
size_t
configMap(std::array<Map_2x2to3x3, 16> &m, std::string_view sv) {
    std::array<size_t, 4> src = {0, 1, 3, 4};
    std::array<size_t, 9> dst = {9, 10, 11, 13, 14, 15, 17, 18, 19};

    auto f = [&sv](size_t &n) { n = sv[n] == '#' ? 1uz : 0uz; };
    std::ranges::for_each(src, f);
    std::ranges::for_each(dst, f);

    auto cnt = std::popcount(bitsToId(src));
    for (auto id : getVariants_2x2(src)) {
        m[id] = Map_2x2to3x3 {.bits = dst, .pop_count = cnt};
    }

    return bitsToId(dst);
}

template <typename T>
std::set<size_t>
getVariants_3x3(T const &bit_seq) {
    std::set<size_t> ids;
    auto w {bit_seq};

    for (auto i = 0; i < 8; ++i) {
        ids.insert(bitsToId(w));

        // rotate
        //   0 1 2 --> 6 3 0
        //   3 4 5     7 4 1
        //   6 7 8     8 5 2
        w = {w[6], w[3], w[0], w[7], w[4], w[1], w[8], w[5], w[2]};

        // flip
        //   0 1 2 --> 6 7 8
        //   3 4 5     3 4 5
        //   6 7 8     0 1 2
        if (i == 3) {
            w = {w[6], w[7], w[8], w[3], w[4], w[5], w[0], w[1], w[2]};
        }
    }

    return ids;
}

// Update the 3x3 to 4x4 mapping table
void
configMap(std::array<Map_3x3to4x4, 512> &m, std::string_view sv) {
    std::array<size_t, 9> src = {0, 1, 2, 4, 5, 6, 8, 9, 10};
    std::array<size_t, 16> dst = {
        15, 16, 17, 18, 20, 21, 22, 23, 25, 26, 27, 28, 30, 31, 32, 33
    };

    auto f = [&sv](size_t &n) { n = sv[n] == '#' ? 1uz : 0uz; };
    std::ranges::for_each(src, f);
    std::ranges::for_each(dst, f);

    auto cnt = std::popcount(bitsToId(src));
    for (auto id : getVariants_3x3(src)) {
        m[id] = Map_3x3to4x4 {.bits = dst, .pop_count = cnt};
    }

    return;
}

TransGrid
makeTransGrid(
    std::array<Map_2x2to3x3, 16> const &m_2to3,
    std::array<Map_3x3to4x4, 512> const &m_3to4,
    size_t start
) {
    TransGrid result {};
    std::array<size_t, 16> grid_16;
    std::array<size_t, 36> grid_36;
    std::array<size_t, 81> grid_81;
    grid_16.fill(0);
    grid_36.fill(0);
    grid_81.fill(0);

    // step 0
    result.pop_count[0] = m_3to4[start].pop_count;

    // step 1
    grid_16 = m_3to4[start].bits;
    result.pop_count[1] = std::ranges::count(grid_16, 1);

    // step 2
    std::vector<std::pair<size_t, size_t>> const idxmap_1 = {
        {0, 0}, {2, 3}, {8, 18}, {10, 21}
    };
    for (auto const [i, j] : idxmap_1) {
        auto id = bitsToId(std::vector<size_t> {
            grid_16[i], grid_16[i + 1], grid_16[i + 4], grid_16[i + 5]
        });
        auto it = m_2to3[id].bits.cbegin();
        for (auto k = 0uz; k < 3; ++k) {
            grid_36[j + 6 * k] = *it++;
            grid_36[j + 6 * k + 1] = *it++;
            grid_36[j + 6 * k + 2] = *it++;
        }
    }
    result.pop_count[2] = std::ranges::count(grid_36, 1);

    // step 3
    std::map<size_t, long> counter;
    std::vector<std::pair<size_t, size_t>> const idxmap_2 = {
        {0, 0},
        {2, 3},
        {4, 6},
        {12, 27},
        {14, 30},
        {16, 33},
        {24, 54},
        {26, 57},
        {28, 60}
    };
    for (auto const [i, j] : idxmap_2) {
        auto id = bitsToId(std::vector<size_t> {
            grid_36[i], grid_36[i + 1], grid_36[i + 6], grid_36[i + 7]
        });
        auto it = m_2to3[id].bits.cbegin();
        for (auto k = 0uz; k < 3; ++k) {
            grid_81[j + 9 * k] = *it++;
            grid_81[j + 9 * k + 1] = *it++;
            grid_81[j + 9 * k + 2] = *it++;
        }

        auto next_id = bitsToId(m_2to3[id].bits);
        counter[next_id] += 1;
    }
    result.pop_count[3] = std::ranges::count(grid_81, 1);

// P1206R7 (range to container conversion) is not yet fully supported in GCC 14.
#if __cpp_lib_containers_ranges >= 202202L
    result.next_grids.append_range(counter);
#else
    result.next_grids.insert(
        result.next_grids.end(), counter.cbegin(), counter.cend()
    );
#endif

    return result;
}

TransTbl
parse(std::istream &is) {
    // The indices of these arrays are also used as the ID of these structures.
    std::array<Map_2x2to3x3, 16> m_2to3;  // 16 = 2 ^ 4
    std::array<Map_3x3to4x4, 512> m_3to4; // 512 = 2 ^ 9

    // Grid IDs for TransGrid objects creation
    std::vector<size_t> id_group {bitsToId(start_grid)};

    for (std::string line; std::getline(is, line);) {
        switch (line.size()) {
        case 20:
            // 2x2 grid to 3x3 grid
            // note grid IDs of 3x3 grids that be produced from 2x2 grids for later use
            id_group.push_back(configMap(m_2to3, line)); // function overloading
            break;
        case 34:
            // 3x3 grid to 4x4 grid
            configMap(m_3to4, line); // function overloading
            break;
        default:; // no-op
        }
    }

    TransTbl tbl;
    for (auto const id : id_group) {
        tbl[id] = makeTransGrid(m_2to3, m_3to4, id);
    }

    return tbl;
}

long
countUpPixels(TransTbl const &trans_tbl, size_t id, size_t depth) {
    if (depth <= 3) {
        return trans_tbl.at(id).pop_count[depth];
    }

    long acc {0};
    for (auto const [next_id, cnt] : trans_tbl.at(id).next_grids) {
        acc += cnt * countUpPixels(trans_tbl, next_id, depth - 3);
    }

    return acc;
}

long
part1(TransTbl const &trans_tbl) {
    return countUpPixels(trans_tbl, bitsToId(start_grid), 5);
}

long
part2(TransTbl const &trans_tbl) {
    return countUpPixels(trans_tbl, bitsToId(start_grid), 18);
}

} // namespace day21
