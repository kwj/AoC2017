#include "gtest/gtest.h"

import solver;
import day10;

class Day10Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/10/input");
        data = day10::parse(input_ss);

        return;
    }

    static std::string data;
};

std::string Day10Test::data;

TEST_F(Day10Test, Part1) {
    auto result = day10::part1(Day10Test::data);
    EXPECT_EQ(result, 1935);
}

TEST_F(Day10Test, Part2) {
    std::string expect_result = "dc7e7dee710d4c7201ce42713e6b8359";

    auto result = day10::part2(Day10Test::data);
    EXPECT_EQ(result, expect_result);
}
