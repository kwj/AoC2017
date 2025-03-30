#include "gtest/gtest.h"

import solver;
import day20;

class Day20Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/20/input");
        data = day20::parse(input_ss);

        return;
    }

    static std::vector<day20::Particle> data;
};

std::vector<day20::Particle> Day20Test::data;

TEST_F(Day20Test, Part1) {
    auto result = day20::part1(Day20Test::data);
    EXPECT_EQ(result, 157);
}

TEST_F(Day20Test, Part2) {
    auto result = day20::part2(Day20Test::data);
    EXPECT_EQ(result, 499);
}
