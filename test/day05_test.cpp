#include "gtest/gtest.h"

import solver;
import day05;

class Day05Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/05/input");
        data = day05::parse(input_ss);

        return;
    }

    static std::vector<long> data;
};

std::vector<long> Day05Test::data;

TEST_F(Day05Test, Part1) {
    auto result = day05::part1(Day05Test::data);
    EXPECT_EQ(result, 354121);
}

TEST_F(Day05Test, Part2) {
    auto result = day05::part2(Day05Test::data);
    EXPECT_EQ(result, 27283023);
}

