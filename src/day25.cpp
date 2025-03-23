module;

#include <algorithm>
#include <array>
#include <istream>
#include <map>
#include <span>
#include <string>
#include <tuple>
#include <vector>

import util;

// --------
export module day25;

export namespace day25 {

struct Op {
    size_t v; // a value to write (it is also used as an index)
    size_t next; // next state (it is also used as an index)
    bool dir; // true: right, false: left
};

struct Head {
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

std::tuple<long> solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data)};
}

// virtual tape:
//   it acts like as a stack except it returns a value N
//   when calling read() on empty queue.
template <typename T, T N>
class Tape {
public:
    Tape() { data = std::vector<T>(); };
    void write(T v);
    T read();
private:
    std::vector<T> data;
};

template <typename T, T N>
void Tape<T, N>::write(T v) {
    data.push_back(v);

    return;
}

template <typename T, T N>
T Tape<T, N>::read() {
    if (data.empty()) {
        return N;
    } else {
        auto v = data.back();
        data.pop_back();

        return v;
    }
}

Head parse(std::istream &is) {
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

            actions[crnt] = {v, next, dir};
        }
        tmp[state] = actions;
    }

    for (auto &[_, acts] : tmp) {
        rules.push_back(acts);
    }

    return {start, steps, rules};
}

unsigned long run(size_t state, long steps, std::vector<std::array<Op, 2>> &rules) {
    auto tape_l = Tape<size_t, 0uz>();
    auto tape_r = Tape<size_t, 0uz>();
    auto counter{0ul};
    auto curr{0uz};

    while (steps-- > 0) {
        auto [v, next_state, dir] = rules[state][curr];
        counter += v - curr;

        // write a new value on the tape, move the cursor to the right/left and
        // read a next value from the tape
        if (dir) {
            // right
            tape_l.write(v);
            curr = tape_r.read();
        } else {
            // left
            tape_r.write(v);
            curr = tape_l.read();
        }

        state = next_state;
    }

    return counter;
}

unsigned long part1(Head &input) {
    return run(input.init_state, input.steps, input.rules);
}

std::string part2() {
    return "There's nothing.";
}

} // namespace day25
