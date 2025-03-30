#include "gtest/gtest.h"

import solver;
import day01;

class Day01Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/01/input");
        data = day01::parse(input_ss);

        return;
    }

    static std::string data;
};

std::string Day01Test::data;

TEST_F(Day01Test, Part1) {
    auto result = day01::part1(Day01Test::data);
    EXPECT_EQ(result, 997);
}

TEST_F(Day01Test, Part2) {
    auto result = day01::part2(Day01Test::data);
    EXPECT_EQ(result, 1358);
}
