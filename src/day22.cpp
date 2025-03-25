module;

#include <complex>
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

struct ComplexHash {
    std::size_t operator()(std::complex<int> const &key) const {
        auto h = std::hash<long> {};
        auto r = static_cast<long>(key.real());
        auto i = static_cast<long>(key.imag());

        return h((r & 0xFFFF) << 16 | (i & 0xFFFF));
    }
};

struct InitialGrid {
    std::unordered_map<std::complex<int>, int, ComplexHash> grid;
    std::complex<int> pos;
};

std::tuple<long, long> solve(std::istream &is);
InitialGrid parse(std::istream &is);
long part1(InitialGrid const &init_grid);
long part2(InitialGrid const &init_grid);

} // namespace day22

// --------
module :private;

namespace day22 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

[[maybe_unused]] constexpr int FLAGGED = 0;
constexpr int CLEAN = 1;
[[maybe_unused]] constexpr int WEAK = 2;
constexpr int INFECTED = 3;

InitialGrid
parse(std::istream &is) {
    std::vector<std::vector<char>> work;
    for (std::string line; std::getline(is, line);) {
        work.push_back(line | std::ranges::to<std::vector<char>>());
    }

    std::unordered_map<std::complex<int>, int, ComplexHash> grid;
    int y {0};
    for (auto chars : work) {
        for (auto [x, ch] : std::views::zip(std::views::iota(0), chars)) {
            if (ch == '#') {
                grid[{x, y}] = INFECTED;
            }
        }
        ++y;
    }

    return {
        grid, std::complex<int>(static_cast<int>(work[0].size()) / 2, y / 2)
    };
}

long
simulate(InitialGrid const &init_grid, long iteration, int delta) {
    constexpr std::complex<int> turn_cw(0, 1), turn_ccw(0, -1), turn_180(-1, 0),
        straight(1, 0);
    std::vector<std::complex<int>> dir_tbl {
        turn_180, turn_ccw, straight, turn_cw
    };
    std::complex<int> dir(0, -1);

    auto grid {init_grid.grid};
    auto pos {init_grid.pos};
    long result {0};

    while (iteration-- > 0) {
        auto [it, _] = grid.try_emplace(pos, CLEAN);
        auto crnt_state = it->second;
        auto new_state = (crnt_state + delta) & 0b11;

        grid[pos] = new_state;
        if (new_state == INFECTED) {
            ++result;
        }

        dir *= dir_tbl[static_cast<size_t>(crnt_state)];
        pos += dir;
    }

    return result;
}

long
part1(InitialGrid const &init_grid) {
    return simulate(init_grid, 10'000, 2);
}

long
part2(InitialGrid const &init_grid) {
    return simulate(init_grid, 10'000'000, 1);
}

} // namespace day22
