module;

#include <algorithm>
#include <cctype>
#include <concepts>
#include <cstddef>
#include <istream>
#include <iterator>
#if __has_include(<mdspan>) // <mdspan> is not supported in GCC 14.
#include <mdspan>
#endif
#include <ranges>
#include <set>
#include <sstream>
#include <stdexcept>
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

// Note:
// Must keep indexes of the grid in the correct range. This implementation depends on
// the outer edges of the input data are all blank ' ' except for the starting point.
// I therefore skipped to check if the indexes are out of range.
#if __cpp_lib_mdspan >= 202207L
std::pair<std::string, long>
followPath(std::vector<char> const &grid_data, long rows, long cols) {
    // Use signed integer instead of size_t for the index type because casting is troublesome.
    std::mdspan<const char, std::dextents<long, 2>> const grid(
        grid_data.data(), rows, cols
    );
    long r {0};
    long c = std::ranges::distance(
        grid_data.cbegin(), std::ranges::find(grid_data, '|')
    );
    long dr {1}, dc {0};

    std::ostringstream oss;
    long cnt {0};
    while (grid[r, c] != ' ') {
        if (grid[r, c] == '+') {
            std::ranges::swap(dr, dc);
            if (grid[r + dr, c + dc] == ' ') {
                dr = -dr;
                dc = -dc;
            }
        } else if (std::isalpha(grid[r, c])) {
            oss << grid[r, c];
        }

        r += dr;
        c += dc;
        ++cnt;
    }

    return {oss.str(), cnt};
}
#else
std::pair<std::string, long>
followPath(std::vector<char> const &grid_data, long cols) {
    std::vector<std::vector<char>> grid;
    for (auto it = grid_data.cbegin(); it != grid_data.cend();
         std::ranges::advance(it, cols)) {
        grid.push_back(
            std::ranges::subrange(it, it + cols) |
            std::ranges::to<std::vector<char>>()
        );
    }

    auto r {0uz};
    auto c = static_cast<size_t>(std::ranges::distance(
        grid_data.cbegin(), std::ranges::find(grid_data, '|')
    ));
    int dr {1}, dc {0};

    std::ostringstream oss;
    long cnt {0};
    while (grid[r][c] != ' ') {
        if (grid[r][c] == '+') {
            std::ranges::swap(dc, dr);
            if (grid[r + static_cast<size_t>(-dr)]
                    [c + static_cast<size_t>(-dc)] != ' ') {
                dr = -dr;
                dc = -dc;
            }
        } else if (std::isalpha(grid[r][c])) {
            oss << grid[r][c];
        }

        r += static_cast<size_t>(dr);
        c += static_cast<size_t>(dc);
        ++cnt;
    }

    return {oss.str(), cnt};
}
#endif

std::pair<std::string, long>
parse(std::istream &is) {
    std::vector<char> data;
    std::set<long> cols;
    [[maybe_unused]] long rows {0};

    for (std::string line; std::getline(is, line);) {
// P1206R7 (range to container conversion) is not yet fully supported in GCC 14.
#if __cpp_lib_containers_ranges >= 202202L
        data.append_range(line);
#else
        data.insert(data.end(), line.cbegin(), line.cend());
#endif
        cols.insert(std::ssize(line));
        ++rows;
    }

    if (cols.size() != 1) {
        throw std::runtime_error("Invalid format (number of columns)");
    }

#if __cpp_lib_mdspan >= 202207L
    return followPath(data, rows, *cols.cbegin());
#else
    return followPath(data, *cols.cbegin());
#endif
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
