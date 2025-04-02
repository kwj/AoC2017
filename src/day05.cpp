module;

#include <cstddef>
#include <functional>
#include <istream>
#include <string>
#include <tuple>
#include <vector>

// --------
export module day05;

export namespace day05 {

std::tuple<long, long> solve(std::istream &is);
std::vector<long> parse(std::istream &is);
long part1(std::vector<long> const &insts);
long part2(std::vector<long> const &insts);

} // namespace day05

// --------
module :private;

namespace day05 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

std::vector<long>
parse(std::istream &is) {
    std::vector<long> result;

    for (std::string line; std::getline(is, line);) {
        result.push_back(std::stol(line));
    }

    return result;
}

long
run(std::vector<long> insts, std::function<long(long)> f) {
    long steps {0};
    size_t idx {0};

    while (idx < insts.size()) {
        auto offset = insts[idx];
        insts[idx] = f(insts[idx]);
        idx += static_cast<size_t>(offset);
        ++steps;
    }

    return steps;
}

long
part1(std::vector<long> const &insts) {
    auto f = [](long n) { return n + 1; };

    return run(insts, f);
}

long
part2(std::vector<long> const &insts) {
    auto f = [](long n) { return n >= 3 ? n - 1 : n + 1; };

    return run(insts, f);
}

} // namespace day05
