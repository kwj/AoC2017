#include "gtest/gtest.h"

import solver;
import day04;

class Day04Test : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        auto input_ss = solver::getInput("./inputs/04/input");
        data = day04::parse(input_ss);

        return;
    }

    static std::vector<day04::Passphrase> data;
};

std::vector<day04::Passphrase> Day04Test::data;

TEST_F(Day04Test, Part1) {
    auto result = day04::part1(Day04Test::data);
    EXPECT_EQ(result, 477);
}

TEST_F(Day04Test, Part2) {
    auto result = day04::part2(Day04Test::data);
    EXPECT_EQ(result, 167);
}
