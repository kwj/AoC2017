#include "gtest/gtest.h"

import solver;
import day17;

class Day17Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/17/input");
        data = day17::parse(input_ss);

        return;
    }

    static long data;
};

long Day17Test::data;

TEST_F(Day17Test, Part1) {
    auto result = day17::part1(Day17Test::data);
    EXPECT_EQ(result, 1506);
}

TEST_F(Day17Test, Part2) {
    auto result = day17::part2(Day17Test::data);
    EXPECT_EQ(result, 39479736);
}

