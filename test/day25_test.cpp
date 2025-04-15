#include "gtest/gtest.h"

import solver;
import day25;

class Day25Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/25/input");
        data = day25::parse(input_ss);

        return;
    }

    static day25::Input data;
};

day25::Input Day25Test::data;

TEST_F(Day25Test, Part1) {
    auto result = day25::part1(Day25Test::data);
    EXPECT_EQ(result, 3578);
}
