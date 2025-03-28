module;

#include <algorithm>
#include <array>
#include <istream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

import util;

// --------
export module day06;

export namespace day06 {

constexpr long N_BANKS {16};
using MemoryBanks = std::array<long, N_BANKS>;

std::tuple<long, long> solve(std::istream &is);
MemoryBanks parse(std::istream &is);
long part1(MemoryBanks const &banks);
long part2(MemoryBanks const &banks);

} // namespace day06

// --------
module :private;

namespace day06 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

MemoryBanks
parse(std::istream &is) {
    MemoryBanks banks;
    std::string line;

    std::getline(is, line);
    auto vn = util::getNumbers(line);
    std::ranges::copy(vn.cbegin(), vn.cbegin() + N_BANKS, banks.begin());

    return banks;
}

void
redistribute(MemoryBanks &banks) {
    auto it = std::ranges::max_element(banks);
    auto q = *it / N_BANKS;

    for (auto r = std::exchange(*it, 0) % N_BANKS; r-- > 0; (*it)++) {
        if (++it == banks.end()) {
            it = banks.begin();
        }
    }

    if (q > 0) {
        std::ranges::for_each(banks, [&](long &n) { n += q; });
    }

    return;
}

// https://en.wikipedia.org/wiki/Cycle_detection#Brent's_algorithm
std::pair<long, long>
brent(MemoryBanks const &banks) {
    MemoryBanks tortoise {banks};
    MemoryBanks hare {banks};

    redistribute(hare);
    long lam {1}; // cycle length
    long power {1};
    while (tortoise != hare) {
        if (power == lam) {
            tortoise = hare;
            power *= 2;
            lam = 0;
        }
        redistribute(hare);
        ++lam;
    }

    tortoise = banks;
    hare = banks;
    for (auto i = 0l; i < lam; ++i) {
        redistribute(hare);
    }
    long mu {0}; // start index of cycle
    while (tortoise != hare) {
        redistribute(tortoise);
        redistribute(hare);
        ++mu;
    }

    return {mu, lam};
}

long
part1(MemoryBanks const &banks) {
    auto const [mu, lam] = brent(banks);

    return mu + lam;
}

long
part2(MemoryBanks const &banks) {
    auto const [_, lam] = brent(banks);

    return lam;
}

} // namespace day06
