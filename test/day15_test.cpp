#include "gtest/gtest.h"

import solver;
import day15;

class Day15Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/15/input");
        data = day15::parse(input_ss);

        return;
    }

    static std::pair<long, long> data;
};

std::pair<long, long> Day15Test::data;

TEST_F(Day15Test, Part1) {
    auto result = day15::part1(Day15Test::data);
    EXPECT_EQ(result, 592);
}

TEST_F(Day15Test, Part2) {
    auto result = day15::part2(Day15Test::data);
    EXPECT_EQ(result, 320);
}

