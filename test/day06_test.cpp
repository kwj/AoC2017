#include "gtest/gtest.h"

import solver;
import day06;

class Day06Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/06/input");
        data = day06::parse(input_ss);

        return;
    }

    static day06::MemoryBanks data;
};

day06::MemoryBanks Day06Test::data;

TEST_F(Day06Test, Part1) {
    auto result = day06::part1(Day06Test::data);
    EXPECT_EQ(result, 7864);
}

TEST_F(Day06Test, Part2) {
    auto result = day06::part2(Day06Test::data);
    EXPECT_EQ(result, 1695);
}
