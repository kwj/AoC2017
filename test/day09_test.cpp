#include "gtest/gtest.h"

import solver;
import day09;

class Day09Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/09/input");
        data = day09::parse(input_ss);

        return;
    }

    static std::pair<long, long> data;
};

std::pair<long, long> Day09Test::data;

TEST_F(Day09Test, Part1) {
    auto result = day09::part1(Day09Test::data);
    EXPECT_EQ(result, 11898);
}

TEST_F(Day09Test, Part2) {
    auto result = day09::part2(Day09Test::data);
    EXPECT_EQ(result, 5601);
}
