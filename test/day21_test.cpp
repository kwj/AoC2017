#include "gtest/gtest.h"

import solver;
import day21;

class Day21Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/21/input");
        data = day21::parse(input_ss);

        return;
    }

    static day21::TransMap data;
};

day21::TransMap Day21Test::data;

TEST_F(Day21Test, Part1) {
    auto result = day21::part1(Day21Test::data);
    EXPECT_EQ(result, 173);
}

TEST_F(Day21Test, Part2) {
    auto result = day21::part2(Day21Test::data);
    EXPECT_EQ(result, 2456178);
}
