#include "gtest/gtest.h"

import solver;
import day11;

class Day11Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/11/input");
        data = day11::parse(input_ss);

        return;
    }

    static std::vector<day11::HexPos> data;
};

std::vector<day11::HexPos> Day11Test::data;

TEST_F(Day11Test, Part1) {
    auto result = day11::part1(Day11Test::data);
    EXPECT_EQ(result, 685);
}

TEST_F(Day11Test, Part2) {
    auto result = day11::part2(Day11Test::data);
    EXPECT_EQ(result, 1457);
}

