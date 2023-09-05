#include "gtest/gtest.h"

#include "gmt/math/2d.h"

using namespace gmt;

TEST(vec2, glm)
{
    glm::vec2 source{ 13.0f, 37.0f };
    gmt::vec2 v{ source };

    EXPECT_EQ(v.x, source.x);
    EXPECT_EQ(v.y, source.y);
}

TEST(vec2, vector)
{
    const float x{ 9.25f };
    const float y{ 11.5f };
    auto v = gmt::vector(x, y);

    EXPECT_EQ(v.x, x);
    EXPECT_EQ(v.y, y);
}