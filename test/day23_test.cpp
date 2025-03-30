#include "gtest/gtest.h"

import solver;
import day23;

class Day23Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/23/input");
        data = day23::parse(input_ss);

        return;
    }

    static long data;
};

long Day23Test::data;

TEST_F(Day23Test, Part1) {
    auto result = day23::part1(Day23Test::data);
    EXPECT_EQ(result, 4225);
}

TEST_F(Day23Test, Part2) {
    auto result = day23::part2(Day23Test::data);
    EXPECT_EQ(result, 905);
}
