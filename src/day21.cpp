/*

Example rules
-------------
#./.. => .#./#../##.
##/#. => #../.##/#..
##/.. => #../#.#/..#
.#/#. => .##/#.#/#..
##/## => #.#/.#./#.#
.#./..#/#.. => ###./.###/..#./##.#

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
I therefore decided to create a conversion table based on three iterations and
used recursion to find the answer.

std::array<ConvGrid, 512> conv_tbl;
  Conversion table with 3x3 grids as indexes. An index is obtained as follows.

    .#.
    ..# --> {0, 1, 0, 0, 0, 1, 1, 1, 1} --> 0b010001111 -> 143
    ###

struct ConvGrid {
    std::array<long, 4> pop_count;
    std::vector<std::pair<size_t, long>> next_grids;
};

  pop_count:
    Number of lit pixels after 0 to 3 iterations
  next_grids:
    Information on nine 3x3 grids after three iterations.
    In the above example, it would look like this

      {{236, 2}, {297, 3}, {166, 3}, {341, 1}}
*/

module;

#include <algorithm>
#include <array>
#include <bit>
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

struct ConvGrid {
    std::array<long, 4> pop_count;
    std::vector<std::pair<size_t, long>> next_grids;
};

std::tuple<long, long> solve(std::istream &is);
std::array<ConvGrid, 512> parse(std::istream &is);
long part1(std::array<ConvGrid, 512> const &conv_tbl);
long part2(std::array<ConvGrid, 512> const &conv_tbl);

} // namespace day21

// --------
module :private;

namespace day21 {

struct Map_2to3 {
    std::array<size_t, 9> bits;
    long pop_count;
};

struct Map_3to4 {
    std::array<size_t, 16> bits;
    long pop_count;
};

std::tuple<long, long> solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

template <typename T>
size_t bitsToId(T const &bit_seq) {
    size_t id{0};

    for (auto const b : bit_seq) {
        id = (id << 1) | b;
    }

    return id;
}

std::set<size_t> getVariant2(std::vector<size_t> const &bit_seq) {
    std::set<size_t> ids;
    auto w{bit_seq};

    for (auto i{0}; i < 8; ++i) {
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

std::set<size_t> update2to3(std::array<Map_2to3, 16> &m_2to3, std::string_view sv) {
    std::vector<size_t> src;
    std::array<size_t, 9> dst;
    auto src_idxes = std::vector<size_t>{0, 1, 3, 4};
    auto dst_idxes = std::vector<size_t>{9, 10, 11, 13, 14, 15, 17, 18, 19};

    for (auto const idx : src_idxes) {
        if (sv[idx] == '#') {
            src.push_back(1);
        } else {
            src.push_back(0);
        }
    }
    auto it = dst.begin();
    for (auto const idx : dst_idxes) {
        if (sv[idx] == '#') {
            *(it++) = 1;
        } else {
            *(it++) = 0;
        }
    }

    auto ids = getVariant2(src);
    auto cnt = std::popcount(bitsToId(src));
    for (auto i : ids) {
        m_2to3[i] = Map_2to3(dst, cnt);
    }

    return ids;
}

std::set<size_t> getVariant3(std::vector<size_t> const &bit_seq) {
    std::set<size_t> ids;
    auto w{bit_seq};

    for (auto i{0}; i < 8; ++i) {
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

std::set<size_t> update3to4(std::array<Map_3to4, 512> &m_3to4, std::string_view sv) {
    std::vector<size_t> src;
    std::array<size_t, 16> dst;
    auto src_idxes = std::vector<size_t>{0, 1, 2, 4, 5, 6, 8, 9, 10};
    auto dst_idxes = std::vector<size_t>{15, 16, 17, 18, 20, 21, 22, 23, 25, 26, 27, 28, 30, 31, 32, 33};

    for (auto const idx : src_idxes) {
        if (sv[idx] == '#') {
            src.push_back(1);
        } else {
            src.push_back(0);
        }
    }
    auto it = dst.begin();
    for (auto const idx : dst_idxes) {
        if (sv[idx] == '#') {
            *(it++) = 1;
        } else {
            *(it++) = 0;
        }
    }

    auto ids = getVariant3(src);
    auto cnt = std::popcount(bitsToId(src));
    for (auto i : ids) {
        m_3to4[i] = Map_3to4(dst, cnt);
    }

    return ids;
}

ConvGrid makeConvGrid(std::array<Map_2to3, 16> const &m_2to3, std::array<Map_3to4, 512> const &m_3to4, size_t start) {
    ConvGrid result{};
    std::array<size_t, 16> grid1;
    std::array<size_t, 36> grid2;
    std::array<size_t, 81> grid3;
    grid1.fill(0);
    grid2.fill(0);
    grid3.fill(0);

    // step 0
    result.pop_count[0] = m_3to4[start].pop_count;

    // step 1
    grid1 = m_3to4[start].bits;
    result.pop_count[1] = std::count(grid1.begin(), grid1.end(), 1);

    // step 2
    std::vector<std::pair<size_t, size_t>> idxmap_1to2 = {
        {0, 0}, {2, 3},
        {8, 18}, {10, 21}
    };
    for (auto const [i, j] : idxmap_1to2) {
        auto id = bitsToId(std::vector<size_t>{grid1[i], grid1[i+1], grid1[i+4], grid1[i+5]});
        auto it = m_2to3[id].bits.begin();
        for (size_t k{0}; k < 3; ++k) {
            grid2[j + 6 * k] = *it++;
            grid2[j + 6 * k + 1] = *it++;
            grid2[j + 6 * k + 2] = *it++;
        }
    }
    result.pop_count[2] = std::count(grid2.begin(), grid2.end(), 1);

    // step 3
    std::map<size_t, long> counter;
    std::vector<std::pair<size_t, size_t>> idxmap_2to3 = {
        {0, 0}, {2, 3}, {4, 6},
        {12, 27}, {14, 30}, {16, 33},
        {24, 54}, {26, 57}, {28, 60}
    };
    for (auto const [i, j] : idxmap_2to3) {
        auto id = bitsToId(std::vector<size_t>{grid2[i], grid2[i+1], grid2[i+6], grid2[i+7]});
        auto it = m_2to3[id].bits.begin();
        for (size_t k{0}; k < 3; ++k) {
            grid3[j + 9 * k] = *it++;
            grid3[j + 9 * k + 1] = *it++;
            grid3[j + 9 * k + 2] = *it++;
        }

        auto next_id = bitsToId(m_2to3[id].bits);
        counter[next_id] += 1;
    }
    result.pop_count[3] = std::count(grid3.begin(), grid3.end(), 1);

    for (auto const [id, cnt] : counter) {
        result.next_grids.push_back({id, cnt});
    }

    return result;
}

std::array<ConvGrid, 512> parse(std::istream &is) {
    std::array<Map_2to3, 16> m_2to3;  // 16 = 2 ^ 4
    std::array<Map_3to4, 512> m_3to4;  // 512 = 2 ^ 9
    std::vector<std::set<size_t>> id_grp2;
    std::vector<std::set<size_t>> id_grp3;

    for (std::string line; std::getline(is, line);) {
        switch (line.size()) {
            case 20:
                id_grp2.push_back(update2to3(m_2to3, line));
                break;
            case 34:
                id_grp3.push_back(update3to4(m_3to4, line));
                break;
        }
    }

    std::array<ConvGrid, 512> result;
    for (auto const &ids: id_grp3) {
        auto primary_id = *ids.begin();
        auto conv_grid = makeConvGrid(m_2to3, m_3to4, primary_id);
        for (auto const id : ids) {
            result[id] = conv_grid;
        }
    }

    return result;
}

long countUpPixels(std::array<ConvGrid, 512> const &conv_tbl, size_t id, size_t depth) {
    if (depth <= 3) {
        return conv_tbl[id].pop_count[depth];
    }

    long acc{0};
    for (auto const [next_id, cnt] : conv_tbl[id].next_grids) {
        acc += cnt * countUpPixels(conv_tbl, next_id, depth - 3);
    }

    return acc;
}

//  .#.
//  ..# --> {0, 1, 0, 0, 0, 1, 1, 1, 1}
//  ###

long part1(std::array<ConvGrid, 512> const &conv_tbl) {
    std::vector<size_t> start_grid = {0, 1, 0, 0, 0, 1, 1, 1, 1};

    return countUpPixels(conv_tbl, bitsToId(start_grid), 5);
}

long part2(std::array<ConvGrid, 512> const &conv_tbl) {
    std::vector<size_t> start_grid = {0, 1, 0, 0, 0, 1, 1, 1, 1};

    return countUpPixels(conv_tbl, bitsToId(start_grid), 18);
}

}  // module day21
