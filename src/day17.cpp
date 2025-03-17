module;

#include <istream>
#include <string>
#include <tuple>
#include <vector>

import util;

// --------
export module day17;

export namespace day17 {

std::tuple<long, long> solve(std::istream &is);
long parse(std::istream &is);
long part1(long step);
long part2(long step);

} // namespace day17

// --------
module :private;

namespace day17 {

constexpr long TARGET{2017};

std::tuple<long, long> solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

long parse(std::istream &is) {
    return std::stoi(util::getLine(is));
}

long part1(long step) {
    std::vector<long> buf{0};
    buf.reserve(TARGET + 1);

    long pos{0};
    for (long i{1}; i <= TARGET; ++i) {
        pos = (pos + step) % static_cast<long>(buf.size()) + 1;
        buf.insert(buf.begin() + static_cast<std::vector<long>::difference_type>(pos), i);
    }

    return buf[static_cast<std::vector<long>::size_type>(pos + 1) % buf.size()];
}

long part2(long step) {
    constexpr long N_INSERTIONS{50'000'000};
    long buf_size{1};
    long pos{0};
    long result{0};
    long n{0};

    while (n < N_INSERTIONS) {
        auto repeat = (buf_size - pos - 1) / step;
        pos = (pos + (repeat * step) + step) % buf_size + 1;
        buf_size += repeat + 1;
        n += repeat + 1;

        if (pos == 1 && n <= N_INSERTIONS) {
            result = n;
        }
    }

    return result;
}

}  // module day17
