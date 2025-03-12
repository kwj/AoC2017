module;

#include <algorithm>
#include <climits>
#include <functional>
#include <istream>
#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

struct Instruction {
    std::string r1;
    std::string r2;
    std::function<long(long)> op_fn;
    std::function<bool(long)> cond_fn;
};

// --------
export module day08;

export namespace day08 {

std::tuple<long, long> solve(std::istream &is);
std::vector<Instruction> parse(std::istream &is);
long part1(std::vector<Instruction> const &insts);
long part2(std::vector<Instruction> const &insts);

}  // namespace day08

// --------
module :private;

namespace day08 {

std::tuple<long, long> solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

long opInc(long lhs, long rhs) { return lhs + rhs; }

bool cmpGt(long lhs, long rhs) { return lhs > rhs; }
bool cmpGe(long lhs, long rhs) { return lhs >= rhs; }
bool cmpLt(long lhs, long rhs) { return lhs < rhs; }
bool cmpLe(long lhs, long rhs) { return lhs <= rhs; }
bool cmpEq(long lhs, long rhs) { return lhs == rhs; }
bool cmpNe(long lhs, long rhs) { return lhs != rhs; }

std::vector<Instruction> parse(std::istream &is) {
    std::vector<Instruction> insts;
    std::smatch m;
    std::regex re(R"((\w+) (\w+) (-?\d+) if (\w+) (\S+) (-?\d+))");

    for (std::string line; std::getline(is, line);) {
        if (std::regex_match(line, m, re)) {
            auto r1 = m[1].str();
            auto r2 = m[4].str();
            auto op1 = m[2].str() == "inc" ? std::stol(m[3].str()) : -std::stol(m[3].str());
            auto op2 = std::stol(m[6].str());
            auto cond = m[5].str();

            auto op_fn = std::bind(opInc, std::placeholders::_1, op1);

            std::function<bool(long)> cond_fn;
            if (cond == ">") {
                cond_fn = std::bind(cmpGt, std::placeholders::_1, op2);
            } else if (cond == ">=") {
                cond_fn = std::bind(cmpGe, std::placeholders::_1, op2);
            } else if (cond == "<") {
                cond_fn = std::bind(cmpLt, std::placeholders::_1, op2);
            } else if (cond == "<=") {
                cond_fn = std::bind(cmpLe, std::placeholders::_1, op2);
            } else if (cond == "==") {
                cond_fn = std::bind(cmpEq, std::placeholders::_1, op2);
            } else if (cond == "!=") {
                cond_fn = std::bind(cmpNe, std::placeholders::_1, op2);
            }

            insts.push_back({r1, r2, op_fn, cond_fn});
        }
    }

    return insts;
}

long part1(std::vector<Instruction> const &insts) {
    std::unordered_map<std::string, long> regs;

    for (auto &[r1, r2, op_fn, cond_fn] : insts) {
        if (cond_fn(regs[r2])) {
            regs[r1] = op_fn(regs[r1]);
        }
    }

    std::vector<long> values;
    for (auto const &[_, v] : regs) {
        values.push_back(v);
    }

    return *std::max_element(values.begin(), values.end());
}

long part2(std::vector<Instruction> const &insts) {
    std::unordered_map<std::string, long> regs;
    long max_value{LONG_MIN};

    for (auto &[r1, r2, op_fn, cond_fn] : insts) {
        if (cond_fn(regs[r2])) {
            regs[r1] = op_fn(regs[r1]);
            max_value = std::max(max_value, regs[r1]);
        }
    }

    return std::max(max_value, 0L);
}

} // namespace day08
