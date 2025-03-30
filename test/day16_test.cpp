#include "gtest/gtest.h"

import solver;
import day16;

class Day16Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/16/input");
        data = day16::parse(input_ss);

        return;
    }

    static day16::Choreography data;
};

day16::Choreography Day16Test::data;

TEST_F(Day16Test, Part1) {
    std::string expect_result = "bijankplfgmeodhc";

    auto result = day16::part1(Day16Test::data);
    EXPECT_EQ(result, expect_result);
}

TEST_F(Day16Test, Part2) {
    std::string expect_result = "bpjahknliomefdgc";

    auto result = day16::part2(Day16Test::data);
    EXPECT_EQ(result, expect_result);
}
