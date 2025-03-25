module;

#include <istream>
#include <string>
#include <tuple>
#include <utility>

// --------
export module day09;

export namespace day09 {

std::tuple<long, long> solve(std::istream &is);
std::pair<long, long> parse(std::istream &is);
long part1(std::pair<long, long> const &result);
long part2(std::pair<long, long> const &result);

} // namespace day09

// --------
module :private;

namespace day09 {

std::tuple<long, long>
solve(std::istream &is) {
    auto result = parse(is);

    return {part1(result), part2(result)};
}

std::pair<long, long>
parse(std::istream &is) {
    long grp_score {0};
    long n_chars {0};
    long depth {1};
    char ch;

    while (is.get(ch)) {
        switch (ch) {
        case '<':
            while (is.get(ch)) {
                if (ch == '!') {
                    is.get(ch);
                } else if (ch == '>') {
                    break;
                } else {
                    ++n_chars;
                }
            }
            break;
        case '{':
            grp_score += depth;
            ++depth;
            break;
        case '}':
            --depth;
            break;
        }
    }

    return std::make_pair(grp_score, n_chars);
}

long
part1(std::pair<long, long> const &result) {
    return result.first;
}

long
part2(std::pair<long, long> const &result) {
    return result.second;
}

} // namespace day09
