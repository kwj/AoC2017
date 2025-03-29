/*
I decompiled the given input data and observed it.

If debug mode is enabled, this program checks `b`, a seed value, is a
composite number or not. The way to check is to multiply all numbers in
the range [2, b) by each other and determine if the result is equal to `b`.

If debug mode is disabled, this program counts the number of composite
numbers in the sequence [b, b + 17, b + 34, ... , b + 1000 * 17]. The method
of determining composite number is the same as in debug mode.

The following is an equivalent pseudo code, but it uses a naive method
to determine composite number or not. Therefore, if the value of `b` is
large, the process takes longer.

-------------------------------------------
if (DEBUG_MODE == ENABLE) {
    a = 0;
} else {
    a = 1;
}

if (a == 0) {
    b = SEED_VALUE;
    c = b;
} else {
    b = 100000 + 100 * SEED_VALUE;  // MUL op
    c = b + 17000;
}

for (; b <= c; b += 17) {
    f = 1;
    for (d = 2; d < b; ++d) {
        for (e = 2; e < b; ++e) {
            if (d * e == b) {  // MUL op
                f = 0;
            }
        }
    }

    if (f == 0) {
        h += 1;
    }
}
-------------------------------------------

I wrote a program which treated the first 'set b ###' (line 1 in my
given input data) as the seed value. Note other parameters are hard
encoded. Because other input data files which I saw on GitHub had
same values.

[Part 1]
Since the solution is the number of multiplication times when determining
the composite number or not, the answer is `(b - 2) * (b - 2)`.

[Part 2]
I used a little naive primality test to determine a number is composite or not.
*/

module;

#include <cmath>
#include <istream>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>

// --------
export module day23;

export namespace day23 {

std::tuple<long, long> solve(std::istream &is);
long parse(std::istream &is);
long part1(long b);
long part2(long b);

} // namespace day23

// --------
module :private;

namespace day23 {

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

long
parse(std::istream &is) {
    std::ostringstream oss;
    oss << is.rdbuf();
    std::string const content = oss.str();

    std::regex const re(R"(set b (\d+))");
    std::smatch m;
    std::regex_search(content, m, re);
    auto seed = std::stol(m[1].str());

    return seed;
}

bool
isPrime(long n) {
    if (n < 4) {
        return n == 2 || n == 3;
    }

    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }

    auto sq_n = static_cast<long>(std::sqrt(n));
    for (auto i = 5L; i <= sq_n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

long
part1(long b) {
    return (b - 2) * (b - 2);
}

long
part2(long b) {
    b = 100000 + 100 * b;
    long const c = b + 17000;
    long result {0};

    for (; b <= c; b += 17) {
        if (!isPrime(b)) {
            ++result;
        }
    }

    return result;
}

} // namespace day23
