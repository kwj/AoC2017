module;

#include <algorithm>
#include <istream>
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

class Choreography {
  public:
    Choreography();
    Choreography(
        std::vector<size_t> const &op_pos, std::vector<size_t> const &op_ltr
    ) :
        by_pos_tbl(op_pos), by_ltr_tbl(op_ltr) {}
    Choreography dance(Choreography const &other);
    Choreography merge(Choreography const &other);
    std::string show();

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

constexpr size_t N_PROGRAMS {16};

Choreography::Choreography() {
    std::vector<size_t> op_pos(N_PROGRAMS);
    std::vector<size_t> op_ltr(N_PROGRAMS);
    std::iota(op_pos.begin(), op_pos.end(), 0);
    std::iota(op_ltr.begin(), op_ltr.end(), 0);

    by_pos_tbl = op_pos;
    by_ltr_tbl = op_ltr;

    return;
}

Choreography
Choreography::dance(Choreography const &other) {
    return merge(other);
}

Choreography
Choreography::merge(Choreography const &other) {
    decltype(by_pos_tbl) new_p;
    decltype(by_ltr_tbl) new_c;

    for (auto const &i : by_pos_tbl) {
        new_p.push_back(other.by_pos_tbl[i]);
    }
    for (auto const &i : by_ltr_tbl) {
        new_c.push_back(other.by_ltr_tbl[i]);
    }

    return {new_p, new_c};
}

std::string
Choreography::show() {
    std::ostringstream oss;

    for (auto const &i : by_pos_tbl) {
        oss << static_cast<char>(by_ltr_tbl[i] + 'a');
    }

    return oss.str();
}

std::tuple<std::string, std::string>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
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
    std::iota(op_pos.begin(), op_pos.end(), 0);
    std::iota(op_ltr.begin(), op_ltr.end(), 0);

    auto num_it = nums.begin();
    for (auto const &cmd : cmds) {
        if (cmd[0] == 's') {
            // Spin
            auto offset = *num_it++;
            std::rotate(op_pos.begin(), op_pos.end() - offset, op_pos.end());
        } else if (cmd[0] == 'x') {
            // Exchange
            auto p1 = *num_it++;
            auto p2 = *num_it++;
            std::iter_swap(op_pos.begin() + p1, op_pos.begin() + p2);
        } else {
            // Partner
            auto ltr_it1 =
                std::find(op_ltr.begin(), op_ltr.end(), cmd[1] - 'a');
            auto ltr_it2 =
                std::find(op_ltr.begin(), op_ltr.end(), cmd[3] - 'a');
            std::iter_swap(ltr_it1, ltr_it2);
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
