#include "gtest/gtest.h"

import solver;
import day08;

class Day08Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/08/input");
        data = day08::parse(input_ss);

        return;
    }

    static std::vector<day08::Instruction> data;
};

std::vector<day08::Instruction> Day08Test::data;

TEST_F(Day08Test, Part1) {
    auto result = day08::part1(Day08Test::data);
    EXPECT_EQ(result, 4448);
}

TEST_F(Day08Test, Part2) {
    auto result = day08::part2(Day08Test::data);
    EXPECT_EQ(result, 6582);
}
