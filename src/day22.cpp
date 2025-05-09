module;

#include <complex>
#include <cstddef>
#include <functional>
#include <istream>
#include <ranges>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

// --------
export module day22;

export namespace day22 {

using Pos = std::complex<long>;

struct [[nodiscard]] PosHash {
    size_t operator()(Pos const &key) const;
};

using Grid = std::unordered_map<Pos, size_t, PosHash>;

std::tuple<long, long> solve(std::istream &is);
std::pair<Grid, Pos> parse(std::istream &is);
long part1(std::pair<Grid, Pos> const &init);
long part2(std::pair<Grid, Pos> const &init);

} // namespace day22

// --------
module :private;

namespace day22 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

size_t
PosHash::operator()(Pos const &key) const {
    auto h = std::hash<long> {};
    auto r = key.real();
    auto i = key.imag();

    return h((r & 0xFFFF) << 16 | (i & 0xFFFF));
}

// Part 1: 0 -> 2 -> 0 -> 2 -> 0 -> ...
// Part 2: 0 -> 1 -> 2 -> 3 -> 0 -> 1 -> 2 -> ...
constexpr size_t CLEAN = 0;
[[maybe_unused]] constexpr size_t WEAK = 1;
constexpr size_t INFECTED = 2;
[[maybe_unused]] constexpr size_t FLAGGED = 3;

std::pair<Grid, Pos>
parse(std::istream &is) {
    std::vector<std::vector<char>> work;
    for (std::string line; std::getline(is, line);) {
        work.push_back(line | std::ranges::to<std::vector<char>>());
    }

    Grid grid;
    long y {0};
    for (auto const &chars : work) {
        for (auto const [x, c] : std::views::zip(std::views::iota(0L), chars)) {
            if (c == '#') {
                grid[{x, y}] = INFECTED;
            }
        }
        ++y;
    }

    return {grid, Pos(std::ssize(work[0]) / 2, y / 2)};
}

long
simulate(Grid grid, Pos pos, long iteration, size_t delta) {
    constexpr Pos turn_cw(0, 1), turn_ccw(0, -1), turn_180(-1, 0),
        straight(1, 0);
    std::vector<Pos> dir_tbl {turn_ccw, straight, turn_cw, turn_180};
    Pos dir(0, -1);
    long result {0};

    while (iteration-- > 0) {
        // The following two lines can be replaced by 'auto crnt_state = grid[pos];'
        // because CLEAN is equal to 0. However, I left them as is to clarify the intent.
        auto [it, _] = grid.try_emplace(pos, CLEAN);
        auto crnt_state = it->second;

        auto new_state = (crnt_state + delta) & 0b11;

        grid[pos] = new_state;
        if (new_state == INFECTED) {
            ++result;
        }

        dir *= dir_tbl[crnt_state];
        pos += dir;
    }

    return result;
}

long
part1(std::pair<Grid, Pos> const &init) {
    return simulate(init.first, init.second, 10'000, 2);
}

long
part2(std::pair<Grid, Pos> const &init) {
    return simulate(init.first, init.second, 10'000'000, 1);
}

} // namespace day22
