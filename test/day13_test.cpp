#include "gtest/gtest.h"

import solver;
import day13;

class Day13Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/13/input");
        data = day13::parse(input_ss);

        return;
    }

    static std::map<long, std::set<long>> data;
};

std::map<long, std::set<long>> Day13Test::data;

TEST_F(Day13Test, Part1) {
    auto result = day13::part1(Day13Test::data);
    EXPECT_EQ(result, 1844);
}

TEST_F(Day13Test, Part2) {
    auto result = day13::part2(Day13Test::data);
    EXPECT_EQ(result, 3897604);
}
