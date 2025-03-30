#include "gtest/gtest.h"

import solver;
import day22;

class Day22Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/22/input");
        data = day22::parse(input_ss);

        return;
    }

    static std::pair<day22::Grid, day22::Pos> data;
};

std::pair<day22::Grid, day22::Pos> Day22Test::data;

TEST_F(Day22Test, Part1) {
    auto result = day22::part1(Day22Test::data);
    EXPECT_EQ(result, 5182);
}

TEST_F(Day22Test, Part2) {
    auto result = day22::part2(Day22Test::data);
    EXPECT_EQ(result, 2512008);
}
