module;

#include <iostream>
#include <string>
#include <regex>
#include <vector>

// ----
export module util;

export namespace util {

std::string getLine(std::istream &is);
std::vector<int> getNumbers(std::string const &s);
std::vector<int> getUNumbers(std::string const &s);

} // namespace util

// ----
module :private;

namespace util {

std::string getLine(std::istream &is) {
    std::string line;
    std::getline(is, line);

    return line;
}

std::vector<int> getNumbers(std::string const &s) {
    std::string src{s};
    std::regex re(R"(\d+)");
    std::vector<int> result;

    for (std::smatch m; std::regex_search(src, m, re);) {
        result.push_back(std::stoi(m.str()));
        src = m.suffix();
    }

    return result;
}

std::vector<int> getUNumbers(std::string const &s) {
    std::string src{s};
    std::regex re(R"(-?\d+)");
    std::vector<int> result;

    for (std::smatch m; std::regex_search(src, m, re);) {
        result.push_back(std::stoi(m.str()));
        src = m.suffix();
    }

    return result;
}

} // namespace util
