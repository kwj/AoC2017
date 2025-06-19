/*
Example rules
-------------
../.. => ##./##./.##
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
  .#.       ###.
  ..#  =>>  .###
  ###       ..#.
            ##.#

[step 2]
              #..|#..
  ##|#.       .##|.##
  .#|##       #..|#..
  --+--  =>>  ---+---
  ..|#.       #..|.##
  ##|.#       #.#|#.#
              ..#|#..

[step 3]
                 .##|.##|#..
  #.|.#|..       #.#|#.#|#.#
  .#|#.|##       #..|#..|..#
  --+--+--       ---+---+---
  #.|.#|..       #..|.#.|#..
  #.|..|##  =>>  #.#|#..|#.#
  --+--+--       ..#|##.|..#
  #.|##|.#       ---+---+---
  ..|##|..       .#.|#.#|.#.
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
I therefore decided to create a transition map based on three iterations and
used recursion to find the answer.

struct TransGrid {
    std::array<long, 4> pop_count;
    std::vector<std::pair<size_t, long>> next_grids;
};

  pop_count:
    Number of lit pixels after 0 to 3 conversions from a 3x3 grid
  next_grids:
    Nine 3x3 grids after three iterations (key and quantity)

std::map<size_t, TransGrid> trans_map;

  Transition map with a 3x3 grid as a key. A key is made as follows.

    .#.
    ..# --> {0, 1, 0, 0, 0, 1, 1, 1, 1} --> 0b010001111 --> 143
    ###

  Only seven entries are needed at most.
    number of '2x2 to 3x3' patterns(6) + start grid(1) = 7

In the above example, it would look like this:

  trans_map[143] = {
      .pop_count = {5, 10, 17, 39},
      .next_grids = {{236, 2}, {297, 3}, {166, 3}, {341, 1}}
  };
*/

module;

#include <algorithm>
#include <array>
#include <bit>
#include <concepts>
#include <cstddef>
#include <istream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

// --------
export module day21;

export namespace day21 {

struct [[nodiscard]] TransGrid {
    std::array<long, 4> pop_count;
    std::vector<std::pair<size_t, long>> next_grids;
};

using TransMap = std::map<size_t, TransGrid>;

std::tuple<long, long> solve(std::istream &is);
TransMap parse(std::istream &is);
long part1(TransMap const &trans_map);
long part2(TransMap const &trans_map);

} // namespace day21

// --------
module :private;

namespace day21 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

using Grid_2x2 = std::array<size_t, 4>;  // 4 = 2 * 2
using Grid_3x3 = std::array<size_t, 9>;  // 9 = 3 * 3
using Grid_4x4 = std::array<size_t, 16>; // 16 = 4 * 4
using Grid_6x6 = std::array<size_t, 36>; // 36 = 6 * 6
using Grid_9x9 = std::array<size_t, 81>; // 81 = 9 * 9

using Tbl_2x2to3x3 = std::array<Grid_3x3, 16>;  // 16 = 2 ^ 4
using Tbl_3x3to4x4 = std::array<Grid_4x4, 512>; // 512 = 2 ^ 9

//  the start grid (glider)
//    .#.
//    ..# --> {0, 1, 0, 0, 0, 1, 1, 1, 1}
//    ###
Grid_3x3 const start_grid = {0, 1, 0, 0, 0, 1, 1, 1, 1};

template <typename T>
concept BitSeq = std::ranges::random_access_range<T> &&
                 std::is_unsigned_v<std::ranges::range_value_t<T>>;

template <BitSeq T>
size_t
bitsToId(T const &bit_seq) {
    size_t id {0};

    for (auto b : bit_seq) {
        id = (id << 1) | b;
    }

    return id;
}

template <BitSeq T>
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
// note: function overloading
size_t
configMap(Tbl_2x2to3x3 &tbl, std::string_view sv) {
    Grid_2x2 src = {0, 1, 3, 4};
    Grid_3x3 dst = {9, 10, 11, 13, 14, 15, 17, 18, 19};

    auto f = [&sv](size_t &n) { n = sv[n] == '#' ? 1uz : 0uz; };
    std::ranges::for_each(src, f);
    std::ranges::for_each(dst, f);

    for (auto id : getVariants_2x2(src)) {
        tbl[id] = dst;
    }

    return bitsToId(dst);
}

template <BitSeq T>
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
// note: function overloading
void
configMap(Tbl_3x3to4x4 &tbl, std::string_view sv) {
    Grid_3x3 src = {0, 1, 2, 4, 5, 6, 8, 9, 10};
    Grid_4x4 dst = {
        15, 16, 17, 18, 20, 21, 22, 23, 25, 26, 27, 28, 30, 31, 32, 33
    };

    auto f = [&sv](size_t &n) { n = sv[n] == '#' ? 1uz : 0uz; };
    std::ranges::for_each(src, f);
    std::ranges::for_each(dst, f);

    for (auto id : getVariants_3x3(src)) {
        tbl[id] = dst;
    }

    return;
}

TransGrid
makeTransGrid(
    Tbl_2x2to3x3 const &tbl_2to3, Tbl_3x3to4x4 const &tbl_3to4, size_t start
) {
    TransGrid result;

    // step 0
    result.pop_count[0] = std::popcount(start);

    // step 1
    Grid_4x4 grid_16 = tbl_3to4[start];
    result.pop_count[1] = std::ranges::count(grid_16, 1);

    // step 2
    std::vector<std::pair<size_t, ptrdiff_t>> const idxmap_1 = {
        {0, 0}, {2, 3}, {8, 18}, {10, 21}
    };
    Grid_6x6 grid_36 {};
    for (auto const [i, j] : idxmap_1) {
        auto id = bitsToId(
            std::vector<size_t> {
                grid_16[i], grid_16[i + 1], grid_16[i + 4], grid_16[i + 5]
            }
        );
        auto it = tbl_2to3[id].cbegin();
        for (ptrdiff_t k = 0; k < 3; ++k, std::ranges::advance(it, 3)) {
            std::ranges::copy_n(it, 3, grid_36.begin() + j + 6 * k);
        }
    }
    result.pop_count[2] = std::ranges::count(grid_36, 1);

    // step 3
    std::vector<std::pair<size_t, ptrdiff_t>> const idxmap_2 = {
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
    Grid_9x9 grid_81 {};
    std::map<size_t, long> counter;
    for (auto const [i, j] : idxmap_2) {
        auto id = bitsToId(
            std::vector<size_t> {
                grid_36[i], grid_36[i + 1], grid_36[i + 6], grid_36[i + 7]
            }
        );
        auto it = tbl_2to3[id].cbegin();
        for (ptrdiff_t k = 0; k < 3; ++k, std::ranges::advance(it, 3)) {
            std::ranges::copy_n(it, 3, grid_81.begin() + j + 9 * k);
        }

        auto next_id = bitsToId(tbl_2to3[id]);
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

TransMap
parse(std::istream &is) {
    // The indices of these arrays are also used as the ID of these structures.
    Tbl_2x2to3x3 tbl_2to3;
    Tbl_3x3to4x4 tbl_3to4;

    // Grid IDs for TransGrid objects creation
    std::vector<size_t> id_group {bitsToId(start_grid)};

    for (std::string line; std::getline(is, line);) {
        switch (line.size()) {
        case 20:
            // 2x2 grid to 3x3 grid
            // note grid IDs of 3x3 grids that be produced from 2x2 grids for later use
            id_group.push_back(configMap(tbl_2to3, line));
            break;
        case 34:
            // 3x3 grid to 4x4 grid
            configMap(tbl_3to4, line);
            break;
        default:
            break; // no-op
        }
    }

    TransMap trans_map;
    for (auto id : id_group) {
        trans_map[id] = makeTransGrid(tbl_2to3, tbl_3to4, id);
    }

    return trans_map;
}

long
countUpPixels(TransMap const &trans_map, size_t id, size_t depth) {
    if (depth <= 3) {
        return trans_map.at(id).pop_count[depth];
    }

    long acc {0};
    for (auto const [next_id, qty] : trans_map.at(id).next_grids) {
        acc += qty * countUpPixels(trans_map, next_id, depth - 3);
    }

    return acc;
}

long
part1(TransMap const &trans_map) {
    return countUpPixels(trans_map, bitsToId(start_grid), 5);
}

long
part2(TransMap const &trans_map) {
    return countUpPixels(trans_map, bitsToId(start_grid), 18);
}

} // namespace day21
