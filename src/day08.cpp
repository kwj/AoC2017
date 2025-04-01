module;

#include <algorithm>
#include <climits>
#include <format>
#include <functional>
#include <istream>
#include <ranges>
#include <regex>
#include <stdexcept>
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
    long op1;
    long op2;
    std::function<bool(long, long)> cond_fn;
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
bool cmpGT(long lhs, long rhs) { return lhs > rhs; }
bool cmpGE(long lhs, long rhs) { return lhs >= rhs; }
bool cmpLT(long lhs, long rhs) { return lhs < rhs; }
bool cmpLE(long lhs, long rhs) { return lhs <= rhs; }
bool cmpEQ(long lhs, long rhs) { return lhs == rhs; }
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

            std::function<bool(long, long)> cond_fn;
            if (cond == ">") {
                cond_fn = cmpGT;
            } else if (cond == ">=") {
                cond_fn = cmpGE;
            } else if (cond == "<") {
                cond_fn = cmpLT;
            } else if (cond == "<=") {
                cond_fn = cmpLE;
            } else if (cond == "==") {
                cond_fn = cmpEQ;
            } else if (cond == "!=") {
                cond_fn = cmpNE;
            } else {
                throw std::runtime_error(
                    std::format("Invalid comparison operator: {}", cond)
                );
            }

            insts.emplace_back(r1, r2, op1, op2, cond_fn);
        }
    }

    return insts;
}

long
part1(std::vector<Instruction> const &insts) {
    std::unordered_map<std::string, long> regs;

    for (auto const &[r1, r2, op1, op2, cond_fn] : insts) {
        if (cond_fn(regs[r2], op2)) {
            regs[r1] += op1;
        }
    }

    return *std::ranges::max_element(regs | std::views::values);
}

long
part2(std::vector<Instruction> const &insts) {
    std::unordered_map<std::string, long> regs;
    long max_value {0};

    for (auto const &[r1, r2, op1, op2, cond_fn] : insts) {
        if (cond_fn(regs[r2], op2)) {
            regs[r1] += op1;
            max_value = std::max(max_value, regs[r1]);
        }
    }

    return max_value;
}

} // namespace day08
