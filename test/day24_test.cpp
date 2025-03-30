#include "gtest/gtest.h"

import solver;
import day24;

class Day24Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/24/input");
        data = day24::parse(input_ss);

        return;
    }

    static std::pair<long, long> data;
};

std::pair<long, long> Day24Test::data;

TEST_F(Day24Test, Part1) {
    auto result = day24::part1(Day24Test::data);
    EXPECT_EQ(result, 1511);
}

TEST_F(Day24Test, Part2) {
    auto result = day24::part2(Day24Test::data);
    EXPECT_EQ(result, 1471);
}

