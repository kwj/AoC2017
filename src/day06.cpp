module;

#include <algorithm>
#include <array>
#include <istream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

import util;

constexpr long N_BANKS{16};

// --------
export module day06;

export namespace day06 {

std::tuple<long, long> solve(std::istream &is);
std::array<long, N_BANKS> parse(std::istream &is);
long part1(std::array<long, N_BANKS> const &banks);
long part2(std::array<long, N_BANKS> const &banks);

}  // namespace day06

// --------
module :private;

namespace day06 {

std::tuple<long, long> solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

std::array<long, N_BANKS> parse(std::istream &is) {
    std::array<long, N_BANKS> banks;
    std::string line;

    std::getline(is, line);
    auto vn = util::getNumbers(line);
    std::copy(vn.begin(), vn.begin() + N_BANKS, banks.begin());

    return banks;
}

void redistribution(std::array<long, N_BANKS> &banks) {
    auto iter = std::max_element(banks.begin(), banks.end());
    auto q = *iter / N_BANKS;

    for (auto r = std::exchange(*iter, 0) % N_BANKS; r-- > 0; (*iter)++) {
        if (++iter == banks.end()) {
            iter = banks.begin();
        }
    }

    if (q > 0) {
        std::for_each(banks.begin(), banks.end(), [&](long &n){ n += q; });
    }

    return;
}

// https://en.wikipedia.org/wiki/Cycle_detection#Brent's_algorithm
std::pair<long, long> brent(std::array<long, N_BANKS> const &banks) {
    std::array<long, N_BANKS> tortoise;
    std::copy(banks.begin(), banks.end(), tortoise.begin());

    std::array<long, N_BANKS> hare;
    std::copy(banks.begin(), banks.end(), hare.begin());
    redistribution(hare);

    long lam{1}; // cycle length
    long power{1};
    while (tortoise != hare) {
        if (power == lam) {
            std::copy(hare.begin(), hare.end(), tortoise.begin());
            power *= 2;
            lam = 0;
        }
        redistribution(hare);
        ++lam;
    }

    std::copy(banks.begin(), banks.end(), tortoise.begin());
    std::copy(banks.begin(), banks.end(), hare.begin());
    for (long i = 0; i < lam; ++i) {
        redistribution(hare);
    }
    long mu{0}; // start index of cycle
    while (tortoise != hare) {
        redistribution(tortoise);
        redistribution(hare);
        ++mu;
    }

    return {mu, lam};
}

long part1(std::array<long, N_BANKS> const &banks) {
    auto [mu, lam] = brent(banks);

    return mu + lam;
}

long part2(std::array<long, N_BANKS> const &banks) {
    auto [_, lam] = brent(banks);

    return lam;
}

} // namespace day06
