#include "gtest/gtest.h"

import solver;
import day18;

class Day18Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/18/input");
        data = day18::parse(input_ss);

        return;
    }

    static std::vector<long> data;
};

std::vector<long> Day18Test::data;

TEST_F(Day18Test, Part1) {
    auto result = day18::part1(Day18Test::data);
    EXPECT_EQ(result, 9423);
}

TEST_F(Day18Test, Part2) {
    auto result = day18::part2(Day18Test::data);
    EXPECT_EQ(result, 7620);
}

