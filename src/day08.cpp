module;

#include <algorithm>
#include <climits>
#include <functional>
#include <istream>
#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

// --------
export module day08;

export namespace day08 {

struct [[nodiscard]] Instruction {
    std::string r1;
    std::string r2;
    std::function<long(long)> op_fn;
    std::function<bool(long)> cond_fn;
};

std::tuple<long, long> solve(std::istream &is);
std::vector<Instruction> parse(std::istream &is);
long part1(std::vector<Instruction> const &insts);
long part2(std::vector<Instruction> const &insts);

} // namespace day08

// --------
module :private;

namespace day08 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

// clang-format off
long opInc(long lhs, long rhs) { return lhs + rhs; }

bool cmpGT(long lhs, long rhs) { return lhs > rhs; }
bool cmpGE(long lhs, long rhs) { return lhs >= rhs; }
bool cmpLT(long lhs, long rhs) { return lhs < rhs; }
bool cmpLE(long lhs, long rhs) { return lhs <= rhs; }
bool cmpEq(long lhs, long rhs) { return lhs == rhs; }
bool cmpNE(long lhs, long rhs) { return lhs != rhs; }
// clang-format on

std::vector<Instruction>
parse(std::istream &is) {
    std::vector<Instruction> insts;
    std::smatch m;
    std::regex const re(R"((\w+) (\w+) (-?\d+) if (\w+) (\S+) (-?\d+))");

    for (std::string line; std::getline(is, line);) {
        if (std::regex_match(line, m, re)) {
            auto r1 = m[1].str();
            auto r2 = m[4].str();
            auto op1 = m[2].str() == "inc" ? std::stol(m[3].str())
                                           : -std::stol(m[3].str());
            auto op2 = std::stol(m[6].str());
            auto cond = m[5].str();

            auto op_fn = [op1](auto &&ph1) {
                return opInc(std::forward<decltype(ph1)>(ph1), op1);
            };

            std::function<bool(long)> cond_fn;
            if (cond == ">") {
                cond_fn = [op2](auto &&ph1) {
                    return cmpGT(std::forward<decltype(ph1)>(ph1), op2);
                };
            } else if (cond == ">=") {
                cond_fn = [op2](auto &&ph1) {
                    return cmpGE(std::forward<decltype(ph1)>(ph1), op2);
                };
            } else if (cond == "<") {
                cond_fn = [op2](auto &&ph1) {
                    return cmpLT(std::forward<decltype(ph1)>(ph1), op2);
                };
            } else if (cond == "<=") {
                cond_fn = [op2](auto &&ph1) {
                    return cmpLE(std::forward<decltype(ph1)>(ph1), op2);
                };
            } else if (cond == "==") {
                cond_fn = [op2](auto &&ph1) {
                    return cmpEq(std::forward<decltype(ph1)>(ph1), op2);
                };
            } else if (cond == "!=") {
                cond_fn = [op2](auto &&ph1) {
                    return cmpNE(std::forward<decltype(ph1)>(ph1), op2);
                };
            }

            insts.emplace_back(r1, r2, op_fn, cond_fn);
        }
    }

    return insts;
}

long
part1(std::vector<Instruction> const &insts) {
    std::unordered_map<std::string, long> regs;

    for (auto const &[r1, r2, op_fn, cond_fn] : insts) {
        if (cond_fn(regs[r2])) {
            regs[r1] = op_fn(regs[r1]);
        }
    }

    long max_value {LONG_MIN};
    for (auto const &r : regs) {
        max_value = std::max(max_value, r.second);
    }

    return max_value;
}

long
part2(std::vector<Instruction> const &insts) {
    std::unordered_map<std::string, long> regs;
    long max_value {0};

    for (auto const &[r1, r2, op_fn, cond_fn] : insts) {
        if (cond_fn(regs[r2])) {
            regs[r1] = op_fn(regs[r1]);
            max_value = std::max(max_value, regs[r1]);
        }
    }

    return max_value;
}

} // namespace day08
