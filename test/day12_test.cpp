#include "gtest/gtest.h"

import solver;
import day12;

class Day12Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/12/input");
        data = day12::parse(input_ss);

        return;
    }

    static std::vector<long> data;
};

std::vector<long> Day12Test::data;

TEST_F(Day12Test, Part1) {
    auto result = day12::part1(Day12Test::data);
    EXPECT_EQ(result, 283);
}

TEST_F(Day12Test, Part2) {
    auto result = day12::part2(Day12Test::data);
    EXPECT_EQ(result, 195);
}

