#include "gtest/gtest.h"

import solver;
import day14;

class Day14Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/14/input");
        data = day14::parse(input_ss);

        return;
    }

    static std::vector<long> data;
};

std::vector<long> Day14Test::data;

TEST_F(Day14Test, Part1) {
    auto result = day14::part1(Day14Test::data);
    EXPECT_EQ(result, 8214);
}

TEST_F(Day14Test, Part2) {
    auto result = day14::part2(Day14Test::data);
    EXPECT_EQ(result, 1093);
}

