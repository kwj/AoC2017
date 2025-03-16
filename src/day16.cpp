module;

#include <algorithm>
#include <deque>
#include <istream>
#include <numeric>
#include <ranges>
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
    Choreography(std::vector<size_t> const &op_pos, std::vector<size_t> const &op_chr) : pos_tbl(op_pos), chr_tbl(op_chr) {}
    Choreography dance(Choreography const &other);
    Choreography merge(Choreography const &other);
    std::string show();
private:
    std::vector<size_t> pos_tbl;
    std::vector<size_t> chr_tbl;
};

std::tuple<std::string, std::string> solve(std::istream &is);
Choreography parse(std::istream &is);
std::string part1(Choreography const &choreo_tbl);
std::string part2(Choreography const &choreo_tbl);

}  // namespace day16

// --------
module :private;

namespace day16 {

constexpr size_t N_PROGRAMS{16};

Choreography::Choreography() {
    std::vector<size_t> op_pos(N_PROGRAMS);
    std::vector<size_t> op_chr(N_PROGRAMS);
    std::iota(op_pos.begin(), op_pos.end(), 0);
    std::iota(op_chr.begin(), op_chr.end(), 0);

    pos_tbl = op_pos;
    chr_tbl = op_chr;

    return;
}

Choreography Choreography::dance(Choreography const &other) {
    return merge(other);
}

Choreography Choreography::merge(Choreography const &other) {
    decltype(pos_tbl) new_p;
    decltype(chr_tbl) new_c;

    for (auto const &i : pos_tbl) {
        new_p.push_back(other.pos_tbl[i]);
    }
    for (auto const &i : chr_tbl) {
        new_c.push_back(other.chr_tbl[i]);
    }

    return {new_p, new_c};
}

std::string Choreography::show() {
    decltype(pos_tbl) work;

    for (auto const &i : pos_tbl) {
        work.push_back(chr_tbl[i] + 'a');
    }

    return std::ranges::to<std::string>(work);
}

std::tuple<std::string, std::string> solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

Choreography parse(std::istream &is) {
    std::string line;
    std::getline(is, line);
    auto cmds = line | std::views::split(',') | std::ranges::to<std::vector<std::string>>();
    auto nums = util::getNumbers(line);

    std::deque<std::string> dq_cmd(cmds.begin(), cmds.end());
    std::deque<long> dq_num(nums.begin(), nums.end());

    // 'a', 'b' ... --> 0, 1, ...
    std::vector<size_t> op_pos(N_PROGRAMS);
    std::vector<size_t> op_chr(N_PROGRAMS);
    std::iota(op_pos.begin(), op_pos.end(), 0);
    std::iota(op_chr.begin(), op_chr.end(), 0);

    for (auto const &cmd : dq_cmd) {
        if (cmd[0] == 's') {
            // Spin
            auto offset = dq_num.front();
            dq_num.pop_front();
            std::rotate(op_pos.begin(), op_pos.end() - offset, op_pos.end());
        } else if (cmd[0] == 'x') {
            // Exchange
            auto p1 = dq_num.front();
            dq_num.pop_front();
            auto p2 = dq_num.front();
            dq_num.pop_front();
            std::iter_swap(op_pos.begin() + p1, op_pos.begin() + p2);
        } else {
            // Partner
            auto it1 = std::find(op_chr.begin(), op_chr.end(), cmd[1] - 'a');
            auto it2 = std::find(op_chr.begin(), op_chr.end(), cmd[3] - 'a');
            std::iter_swap(it1, it2);
        }
    }

    return {op_pos, op_chr};
}

std::string part1(Choreography const &choreo_tbl) {
    auto dancer = Choreography();

    return dancer.dance(choreo_tbl).show();
}

std::string part2(Choreography const &choreo_tbl) {
    long n_dances{1'000'000'000};
    auto dancer = Choreography();
    auto choreo{choreo_tbl};

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
