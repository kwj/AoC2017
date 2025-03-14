module;

#include <iostream>
#include <regex>
#include <string_view>
#include <vector>

// ----
export module util;

export namespace util {

std::string getLine(std::istream &is);
std::vector<long> getNumbers(std::string_view sv);
std::vector<long> getUNumbers(std::string_view sv);

} // namespace util

// ----
module :private;

namespace util {

std::string getLine(std::istream &is) {
    std::string line;
    std::getline(is, line);

    return line;
}

std::vector<long> getNumbers(std::string_view sv) {
    std::regex re(R"(-?\d+)");
    std::vector<long> result;

    std::match_results<std::string_view::const_iterator> m;
    while (std::regex_search(sv.begin(), sv.end(), m, re)) {
        result.push_back(std::stol(m.str()));
        sv = sv.substr(static_cast<size_t>(m.position(0) + m.length(0)));
    }

    return result;
}

std::vector<long> getUNumbers(std::string_view sv) {
    std::regex re(R"(\d+)");
    std::vector<long> result;

    std::match_results<std::string_view::const_iterator> m;
    while (std::regex_search(sv.begin(), sv.end(), m, re)) {
        result.push_back(std::stol(m.str()));
        sv = sv.substr(static_cast<size_t>(m.position(0) + m.length(0)));
    }

    return result;
}

} // namespace util
