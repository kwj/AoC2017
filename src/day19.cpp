module;

#include <cctype>
#include <istream>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

import util;

// --------
export module day19;

export namespace day19 {

std::tuple<std::string, long> solve(std::istream &is);
std::pair<std::string, long> parse(std::istream &is);
std::string part1(std::pair<std::string, long> &result);
long part2(std::pair<std::string, long> &result);

} // namespace day19

// --------
module :private;

namespace day19 {

std::tuple<std::string, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

/* What a terrible code... */
std::pair<std::string, long>
followPath(std::vector<std::vector<char>> &grid) {
    // find the start position and set the initial direction (down[dx=0,dy=1])
    auto x = static_cast<size_t>(
        std::find(grid[0].begin(), grid[0].end(), '|') - grid[0].begin()
    );
    auto y {0uz};
    int dx {0}, dy {1};

    std::ostringstream oss;
    long cnt {0};
    while (grid[y][x] != ' ') {
        if (grid[y][x] == '+') {
            std::swap(dx, dy);
            auto rev_dx {-dx}, rev_dy {-dy};
            if (grid[y + static_cast<size_t>(rev_dy)]
                    [x + static_cast<size_t>(rev_dx)] != ' ') {
                dx = rev_dx;
                dy = rev_dy;
            }
        } else if (std::isalpha(grid[y][x])) {
            oss << grid[y][x];
        }

        x += static_cast<size_t>(dx);
        y += static_cast<size_t>(dy);
        ++cnt;
    }

    return {oss.str(), cnt};
}

std::pair<std::string, long>
parse(std::istream &is) {
    std::vector<std::vector<char>> grid;
    for (std::string line; std::getline(is, line);) {
        grid.push_back(line | std::ranges::to<std::vector<char>>());
    }

    return followPath(grid);
}

std::string
part1(std::pair<std::string, long> &result) {
    return result.first;
}

long
part2(std::pair<std::string, long> &result) {
    return result.second;
}

} // namespace day19
