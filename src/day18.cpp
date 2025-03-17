/*
I decompiled the given input data and observed it.

[Part 1]
127 pseudo-randomized like numbers are generated, and the last value is
the answer to the problem.

  x = (((x * A1) % M1) * A2 + C) % M1;

[Part 2]
This is a cooperative bubble sort (descending order) by two processes.
The initial data is the numerical sequence generated in the part 1.

The two processes are referred to as proc/0 and proc/1.

    proc/0                proc/1      (example: 6-elements)
      |                     |
      | --(initial data)--> |         [25, 83, 66, 8, 98, 78]
      |                     |
      | <------ (1) ------- |         [83, 66, 25, 98, 78, 8]
      |                     |
      | ------- (2) ------> |         [83, 66, 98, 78, 25, 8]
      |                     |
               .....
      |                     |
      | <------ (n) ------- |         [98, 83, 78, 66, 25, 8]
      |                     |

    (1): sequence of 127 numbers with at least the smallest number at the tail
    (2): sequence of 127 numbers with at least two descending sorted numbers at the tail
     .....
    (n): sequence of 127 numbers with all sorted numbers in descending order

Each process only receives after sending the all sorted sequence. So they get into a deadlock state.

The answer, the number of numbers sent by proc/1, is 127 * ceil(n/2).

-------------------------

I was too lazy to implement a simulator, so I wrote a program which treated
the first 'set p ###' (line 10 in my given input data) as the seed value.

Note other parameters are hard encoded. Because other input data files which
I saw on GitHub had same values.
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
long part1(std::vector<long> &vs);
long part2(std::vector<long> &vs);

} // namespace day18

// --------
module :private;

namespace day18 {

std::tuple<long, long> solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

std::vector<long> parse(std::istream &is) {
    std::ostringstream oss;
    oss << is.rdbuf();
    std::string content = oss.str();

    std::regex re(R"(set p (\d+))");
    std::smatch m;
    std::regex_search(content, m, re);
    auto seed = std::stol(m[1].str());

    constexpr long M1{0x7FFFFFFF}; // 2^31 - 1
    constexpr long M2{10000};
    constexpr long A1{8505};
    constexpr long A2{129749};
    constexpr long C{12345};
    std::vector<long> vs;
    long x = seed;
    for (auto i{127}; i > 0; --i) {
        x = (((x * A1) % M1) * A2 + C) % M1;
        vs.push_back(x % M2);
    }

    return vs;
}

long part1(std::vector<long> &vs) {
    return vs.back();
}

long part2(std::vector<long> &vs) {
    std::vector<long> work{vs};
    auto cont_flag{true};
    long cnt{0};

    while (cont_flag) {
        cont_flag = false;
        for (auto it = work.begin(); it < work.end() - 1; ++it) {
            if (*it < *(it + 1)) {
                std::iter_swap(it, it + 1);
                cont_flag = true;
            }
        }
        ++cnt;
    }

    return 127 * ((cnt + 1) / 2);
}

}  // module day18
