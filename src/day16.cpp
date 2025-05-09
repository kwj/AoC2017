module;

#include <algorithm>
#include <cstddef>
#include <istream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

import util;

// --------
export module day16;

export namespace day16 {

class [[nodiscard]] Choreography {
  public:
    Choreography();
    Choreography(
        std::vector<size_t> const &op_pos, std::vector<size_t> const &op_ltr
    ) :
        by_pos_tbl(op_pos), by_ltr_tbl(op_ltr) {}
    Choreography dance(Choreography const &other) const;
    Choreography merge(Choreography const &other) const;
    [[nodiscard]] std::string show() const;

  private:
    std::vector<size_t> by_pos_tbl;
    std::vector<size_t> by_ltr_tbl;
};

std::tuple<std::string, std::string> solve(std::istream &is);
Choreography parse(std::istream &is);
std::string part1(Choreography const &choreo_tbl);
std::string part2(Choreography const &choreo_tbl);

} // namespace day16

// --------
module :private;

namespace day16 {

std::tuple<std::string, std::string>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

constexpr size_t N_PROGRAMS {16};

Choreography::Choreography() {
    by_pos_tbl.resize(N_PROGRAMS);
    by_ltr_tbl.resize(N_PROGRAMS);

// P2440R1 (ranges::iota) is not yet supported in libc++ 19.
#if __cpp_lib_ranges_iota >= 202202L
    std::ranges::iota(by_pos_tbl, 0);
    std::ranges::iota(by_ltr_tbl, 0);
#else
    std::iota(by_pos_tbl.begin(), by_pos_tbl.end(), 0);
    std::iota(by_ltr_tbl.begin(), by_ltr_tbl.end(), 0);
#endif

    return;
}

Choreography
Choreography::dance(Choreography const &other) const {
    return merge(other);
}

Choreography
Choreography::merge(Choreography const &other) const {
    decltype(by_pos_tbl) new_p;
    decltype(by_ltr_tbl) new_c;

    for (auto i : by_pos_tbl) {
        new_p.push_back(other.by_pos_tbl[i]);
    }
    for (auto i : by_ltr_tbl) {
        new_c.push_back(other.by_ltr_tbl[i]);
    }

    return {new_p, new_c};
}

std::string
Choreography::show() const {
    std::ostringstream oss;

    for (auto i : by_pos_tbl) {
        oss << static_cast<char>(by_ltr_tbl[i] + 'a');
    }

    return oss.str();
}

Choreography
parse(std::istream &is) {
    std::string line;
    std::getline(is, line);
    auto cmds = line | std::views::split(',') |
                std::ranges::to<std::vector<std::string>>();
    auto nums = util::getNumbers(line);

    // 'a', 'b' ... --> 0, 1, ...
    std::vector<size_t> op_pos(N_PROGRAMS);
    std::vector<size_t> op_ltr(N_PROGRAMS);

// P2440R1 (ranges::iota) is not yet supported in libc++ 19.
#if __cpp_lib_ranges_iota >= 202202L
    std::ranges::iota(op_pos, 0);
    std::ranges::iota(op_ltr, 0);
#else
    std::iota(op_pos.begin(), op_pos.end(), 0);
    std::iota(op_ltr.begin(), op_ltr.end(), 0);
#endif

    auto num_it = nums.cbegin();
    for (auto const &cmd : cmds) {
        if (cmd[0] == 's') {
            // Spin
            auto offset = *num_it++;
            std::ranges::rotate(
                op_pos.begin(), op_pos.end() - offset, op_pos.end()
            );
        } else if (cmd[0] == 'x') {
            // Exchange
            auto p1 = *num_it++;
            auto p2 = *num_it++;
            std::ranges::iter_swap(op_pos.begin() + p1, op_pos.begin() + p2);
        } else {
            // Partner
            auto ltr_it1 = std::ranges::find(op_ltr, cmd[1] - 'a');
            auto ltr_it2 = std::ranges::find(op_ltr, cmd[3] - 'a');
            std::ranges::iter_swap(ltr_it1, ltr_it2);
        }
    }

    return {op_pos, op_ltr};
}

std::string
part1(Choreography const &choreo_tbl) {
    auto dancer = Choreography();

    return dancer.dance(choreo_tbl).show();
}

std::string
part2(Choreography const &choreo_tbl) {
    long n_dances {1'000'000'000};
    auto dancer = Choreography();
    auto choreo {choreo_tbl};

    while (n_dances > 0) {
        if ((n_dances & 0b1) != 0) {
            dancer = dancer.dance(choreo);
        }
        choreo = choreo.merge(choreo);
        n_dances >>= 1;
    }

    return dancer.show();
}

} // namespace day16
