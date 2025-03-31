module;

#include <array>
#include <cstddef>
#include <deque>
#include <istream>
#include <iterator>
#include <map>
#include <ranges>
#include <span>
#include <string>
#include <tuple>
#include <vector>

import util;

// --------
export module day25;

export namespace day25 {

struct [[nodiscard]] Op {
    size_t v;    // a value to write (it is also used as an index)
    size_t next; // next state (it is also used as an index)
    bool dir;    // true: right, false: left
};

struct [[nodiscard]] Head {
    size_t init_state;
    long steps;
    std::vector<std::array<Op, 2>> rules;
};

std::tuple<long> solve(std::istream &is);
Head parse(std::istream &is);
unsigned long part1(Head &input);
std::string part2();

} // namespace day25

// --------
module :private;

namespace day25 {

std::tuple<long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data)};
}

// virtual tape
template <typename T, T N>
class [[nodiscard]] Tape {
  public:
    Tape() {
        data = std::deque<T>(1, N);
        it = data.begin();
    }
    T write(T v, bool dir);

  private:
    std::deque<T> data;
    std::deque<T>::iterator it;
};

template <typename T, T N>
T
Tape<T, N>::write(T v, bool dir) {
    *it = v;
    if (dir) {
        // right
        if (std::next(it) == data.end()) {
            data.push_back(N);
            it = std::prev(data.end());
        } else {
            ++it;
        }
    } else {
        // left
        if (it == data.begin()) {
            data.push_front(N);
            it = data.begin();
        } else {
            --it;
        }
    }

    return *it;
}

Head
parse(std::istream &is) {
    std::vector<std::string> lines;
    for (std::string line; std::getline(is, line);) {
        lines.push_back(line);
    }

    auto start = static_cast<unsigned long>(lines[0][15] - 'A');
    auto steps = util::getNumbers(lines[1])[0];

    std::vector<std::array<Op, 2>> rules;
    std::map<size_t, std::array<Op, 2>> tmp;

    auto data = std::span(lines);
    for (auto offset = 3uz; offset < lines.size() - 2; offset += 10) {
        std::array<Op, 2> actions;
        auto block = data.subspan(offset, 10uz);
        auto state = static_cast<size_t>(block[0][9] - 'A');

        for (auto i = 0uz; i < 2; ++i) {
            auto crnt = static_cast<size_t>(block[4 * i + 1][26] - '0');
            auto v = static_cast<unsigned long>(block[4 * i + 2][22] - '0');
            auto dir = block[4 * i + 3][27] == 'r';
            auto next = static_cast<size_t>(block[4 * i + 4][26] - 'A');

            actions[crnt] = {.v = v, .next = next, .dir = dir};
        }
        tmp[state] = actions;
    }

// P1206R7 (range to container conversion) is not yet fully supported in GCC 14.
#if __cpp_lib_containers_ranges >= 202202L
    rules.append_range(std::views::values(tmp));
#else
    for (auto &acts : std::views::values(tmp)) {
        rules.push_back(acts);
    }
#endif

    return {.init_state = start, .steps = steps, .rules = rules};
}

unsigned long
run(size_t state, long steps, std::vector<std::array<Op, 2>> &rules) {
    auto tape = Tape<size_t, 0uz>();
    auto counter {0ul};
    auto curr {0uz};

    while (steps-- > 0) {
        auto [v, next_state, dir] = rules[state][curr];
        counter += v - curr;

        // write a new value on the tape, move the cursor to the right/left and
        // return a next value from the tape
        curr = tape.write(v, dir);

        state = next_state;
    }

    return counter;
}

unsigned long
part1(Head &input) {
    return run(input.init_state, input.steps, input.rules);
}

std::string
part2() {
    return "There's nothing.";
}

} // namespace day25
