#include "gtest/gtest.h"

import solver;
import day02;

class Day02Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/02/input");
        data = day02::parse(input_ss);

        return;
    }

    static std::vector<std::vector<long>> data;
};

std::vector<std::vector<long>> Day02Test::data;

TEST_F(Day02Test, Part1) {
    auto result = day02::part1(Day02Test::data);
    EXPECT_EQ(result, 34925);
}

TEST_F(Day02Test, Part2) {
    auto result = day02::part2(Day02Test::data);
    EXPECT_EQ(result, 221);
}
