#include "gtest/gtest.h"

import solver;
import day07;

class Day07Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/07/input");
        data = day07::parse(input_ss);

        return;
    }

    static std::vector<day07::Disc> data;
};

std::vector<day07::Disc> Day07Test::data;

TEST_F(Day07Test, Part1) {
    std::string expect_result = "xegshds";

    auto result = day07::part1(Day07Test::data);
    EXPECT_EQ(result, expect_result);
}

TEST_F(Day07Test, Part2) {
    auto result = day07::part2(Day07Test::data);
    EXPECT_EQ(result, 299);
}

