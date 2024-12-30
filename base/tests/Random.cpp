#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gmt/utils.h>

using namespace ::testing;

TEST(Random, RandomDistribution) {
    auto t = gmt::utils::randomDistribution(std::uniform_int_distribution{ 1, 5 });
    for (int i = 0; i < 32; i++) {
        EXPECT_THAT(t(), AllOf(Ge(1), Le(5)));
    }
}

TEST(Random, RandomDistribution2) {
    auto t = gmt::utils::randomDistribution(std::uniform_int_distribution{ 1, 1 });
    for (int i = 0; i < 32; i++) {
        EXPECT_EQ(t(), 1);
    }
}