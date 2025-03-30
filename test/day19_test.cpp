#include "gtest/gtest.h"

import solver;
import day19;

class Day19Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/19/input");
        data = day19::parse(input_ss);

        return;
    }

    static std::pair<std::string, long> data;
};

std::pair<std::string, long> Day19Test::data;

TEST_F(Day19Test, Part1) {
    std::string expect_result = "FEZDNIVJWT";

    auto result = day19::part1(Day19Test::data);
    EXPECT_EQ(result, expect_result);
}

TEST_F(Day19Test, Part2) {
    auto result = day19::part2(Day19Test::data);
    EXPECT_EQ(result, 17200);
}
