/*
[Part 2]
from example (step = 3):

- 0  5 (7) 2  4  3  6  1
  buf_size = 8
  pos = 2
  n = 7

- 0| 5 (7) |2  4  3| 6  1
  -         --(1)-- --(2)-
  repeat = (8 - 2 - 1) / 3 + 1 = 2
 ==>
  pos <- (2 + 2 * 3) % 8 + 1 = 0 + 1
  buf_size <- 8 + 2 = 10
  n <- 7 + 2 = 9

results:
- 0 (9) 5  7  2  4  3  #  6  1


next:
- 0| (9)|5  7  2| 4  3  #| 6  1
  -    --(1)--  --(2)--  --(3)-
  repeat = (10 - 1 - 1) / 3 + 1 = 3
 ==>
  pos <- (1 + 3 * 3) % 10 + 1 = 1
  buf_size <- 10 + 3 = 13
  n <- 9 + 3 = 12

results:
- 0 (12)  9  5  7  2  #  4  3  #  #  6  1

and, so on.
----------------

With my given input data, it terminated after a 4,617 times loop.
Naturally, it was dramatically faster than a naive fifty million times loop.

  loop count      n
  --------------------
           ...
     375         375
     376         376
     377         377
     378         379
     379         380
           ...
    1999       34386
    2000       34482
    2001       34578
           ...
    2999      555175
    3000      556722
    3001      558273
           ...
    3999     8963599
    4000     8988567
    4001     9013605
           ...
    4615    49732762
    4616    49871293
    4617    50010210
*/

module;

#include <cstddef>
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

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

long
parse(std::istream &is) {
    return std::stoi(util::getLine(is));
}

long
part1(long step) {
    constexpr long N_INSERTIONS {2017};
    std::vector<long> buf {0};
    buf.reserve(N_INSERTIONS + 1);

    long pos {0};
    for (auto i = 1L; i <= N_INSERTIONS; ++i) {
        pos = (pos + step) % std::ssize(buf) + 1;
        buf.insert(buf.begin() + pos, i);
    }

    return buf[static_cast<size_t>(pos + 1) % buf.size()];
}

long
part2(long step) {
    constexpr long N_INSERTIONS {50'000'000};
    long buf_size {1};
    long pos {0};
    long result {0};
    long n {0};

    while (n < N_INSERTIONS) {
        auto repeat = (buf_size - pos - 1) / step + 1;
        pos = (pos + (repeat * step)) % buf_size + 1;
        buf_size += repeat;
        n += repeat;

        if (pos == 1 && n <= N_INSERTIONS) {
            result = n;
        }
    }

    return result;
}

} // namespace day17
