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
std::vector<unsigned long> getUNumbers(std::string_view sv);

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
    std::vector<long> result;

    std::regex re(R"(-?\d+)");
    auto it = std::regex_token_iterator<std::string_view::const_iterator>{
        sv.cbegin(), sv.cend(), re
    };

    for (decltype(it) last; it != last; ++it) {
        result.push_back(std::stol(*it));
    }

    return result;
}

std::vector<unsigned long> getUNumbers(std::string_view sv) {
    std::vector<unsigned long> result;

    std::regex re(R"(\d+)");
    auto it = std::regex_token_iterator<std::string_view::const_iterator>{
        sv.cbegin(), sv.cend(), re
    };

    for (decltype(it) last; it != last; ++it) {
        result.push_back(std::stoul(*it));
    }

    return result;
}

} // namespace util
