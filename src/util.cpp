module;

#include <iostream>
#include <regex>
#include <string>
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

std::string
getLine(std::istream &is) {
    std::string line;
    std::getline(is, line);

    return line;
}

std::vector<long>
allNumbers(std::string_view sv, std::regex re) {
    std::vector<long> result;

    auto it = std::regex_token_iterator<std::string_view::const_iterator> {
        sv.cbegin(), sv.cend(), re
    };

    // Default std::regex_token_iterator constructor creates the end-of-sequence iterator
    for (decltype(it) const last; it != last; ++it) {
        result.push_back(std::stol(*it));
    }

    return result;
}

std::vector<long>
getNumbers(std::string_view sv) {
    std::regex const re(R"(-?\d+)");

    return allNumbers(sv, re);
}

std::vector<long>
getUNumbers(std::string_view sv) {
    std::regex const re(R"(\d+)");

    return allNumbers(sv, re);
}

} // namespace util
