#include "gtest/gtest.h"

import solver;
import day03;

class Day03Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/03/input");
        data = day03::parse(input_ss);

        return;
    }

    static long data;
};

long Day03Test::data;

TEST_F(Day03Test, Part1) {
    auto result = day03::part1(Day03Test::data);
    EXPECT_EQ(result, 438);
}

TEST_F(Day03Test, Part2) {
    auto result = day03::part2(Day03Test::data);
    EXPECT_EQ(result, 266330);
}

