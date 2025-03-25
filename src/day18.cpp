/*
I decompiled the given input data and observed it.

[Part 1]
127 pseudo-randomized like numbers are generated, and the last value is
the answer to the problem.

  x = (((x * A1) % M1) * A2 + C) % M1;

[Part 2]
This is a cooperative bubble sort (descending order) by two processes.
The initial data is the numerical sequence generated in the part 1.

In the following, for simplicity, consider a sequence of numbers of
length 7 instead of 127. The two processes are referred to as proc/0
and proc/1.

    proc/0                proc/1      (example: 7-elements)
      |                     |
      | --(initial data)--> |         [27, 83, 13, 66, 8, 98, 25]
      |                     |
      | <------ (1) ------- |         [83, 27, 66, 13, 98, 25, 8]  (swap occurred)
      |                     |
      | ------- (2) ------> |         [83, 66, 27, 98, 25, 13, 8]  (swap occurred)
      |                     |
      | <------ (3) ------- |         [83, 66, 98, 27, 25, 13, 8]  (swap occurred)
      |                     |
      | ------- (4) ------> |         [83, 98, 66, 27, 25, 13, 8]  (swap occurred)
      |                     |
      | <------ (5) ------- |         [98, 83, 66, 27, 25, 13, 8]  (swap occurred)
      |                     |
      | ------- (6) ------> |         [98, 83, 66, 27, 25, 13, 8]
      |                     |
      | <------ (7) ------- |         [98, 83, 66, 27, 25, 13, 8]
      |                     |

    ===> Number of times a number is sent in the proc/1 is 7 * 4 = 28.

Each process enters a deadlock state after sending sorted sequences with no elements
swapping occurring.

By the way, there are cases where the proc/0 completes the sort instead of the proc/1.

    proc/0                proc/1      (example: 7-elements)
      |                     |
      | --(initial data)--> |         [27, 98, 13, 66, 8, 83, 25]
      |                     |
      | <------ (1) ------- |         [98, 27, 66, 13, 83, 25, 8]  (swap occurred)
      |                     |
      | ------- (2) ------> |         [98, 66, 27, 83, 25, 13, 8]  (swap occurred)
      |                     |
      | <------ (3) ------- |         [98, 66, 83, 27, 25, 13, 8]  (swap occurred)
      |                     |
      | ------- (4) ------> |         [98, 83, 66, 27, 25, 13, 8]  (swap occurred)
      |                     |
      | <------ (5) ------- |         [98, 83, 66, 27, 25, 13, 8]
      |                     |
      | ------- (6) ------> |         [98, 83, 66, 27, 25, 13, 8]
      |                     |

    ===> Number of times a number is sent in the proc/1 is 7 * 3 = 21.

I had to consider both cases where the proc/1 completes the sort or not. Because I was
too lazy to implement a simulator and translated the given program into an program using
another algorithm.

-------------------------

I wrote a program which treated the first 'set p ###' (line 10 in my given input
data) as the seed value. Note other parameters are hard encoded. Because other input
data files which I saw on GitHub had same values.
*/

module;

#include <algorithm>
#include <istream>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

// --------
export module day18;

export namespace day18 {

std::tuple<long, long> solve(std::istream &is);
std::vector<long> parse(std::istream &is);
long part1(std::vector<long> const &vs);
long part2(std::vector<long> const &vs);

} // namespace day18

// --------
module :private;

namespace day18 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

std::vector<long>
parse(std::istream &is) {
    std::ostringstream oss;
    oss << is.rdbuf();
    std::string content = oss.str();

    std::regex re(R"(set p (\d+))");
    std::smatch m;
    std::regex_search(content, m, re);
    auto seed = std::stoull(m[1].str());

    constexpr unsigned long long M1 {0x7FFFFFFF}; // 2^31 - 1
    constexpr unsigned long long M2 {10000};
    constexpr unsigned long long A1 {8505};
    constexpr unsigned long long A2 {129749};
    constexpr unsigned long long C {12345};
    constexpr long N_ELEMENTS {127};
    std::vector<long> vs;
    vs.reserve(N_ELEMENTS);
    auto x = seed;
    for (auto i = N_ELEMENTS; i > 0; --i) {
        x = (((x * A1) % M1) * A2 + C) % M1;
        vs.push_back(static_cast<long>(x % M2));
    }

    return vs;
}

long
part1(std::vector<long> const &vs) {
    return vs.back();
}

long
part2(std::vector<long> const &vs) {
    std::vector<long> work {vs};
    auto cont_flag {true};
    long cnt {0};

    while (cont_flag) {
        cont_flag = false;
        for (auto it = work.begin(); it < work.end() - 1; ++it) {
            if (*it < *(it + 1)) {
                std::iter_swap(it, it + 1);
                cont_flag = true;
            }
        }

        ++cnt;
        if (cont_flag == true &&
            std::is_sorted(work.begin(), work.end(), std::greater<long> {})) {
            ++cnt;
        }
    }

    return static_cast<long>(work.size()) * ((cnt + 1) / 2);
}

} // namespace day18
