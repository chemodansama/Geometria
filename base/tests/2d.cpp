#include "gtest/gtest.h"

#include <iterator>

#include <glm/gtc/constants.hpp>
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

TEST(make_span, simple)
{
    std::vector<int> x{ 1, 2, 3, 4 };
    const auto s = gmt::make_span(x.data(), x.data() + 3);
    EXPECT_EQ(3, s.size());
    EXPECT_EQ(1, s[0]);
    EXPECT_EQ(2, s[1]);
    EXPECT_EQ(3, s[2]);
}

TEST(MergePoints, simple)
{
    std::vector<gmt::vec2> x{ { 1.0f, 1.0f }, { 2.0f, 2.0f } };
    const auto count = gmt::mergePoints(gmt::make_span(x.begin(), x.end()));
    EXPECT_EQ(count, 2);
}

TEST(MergePoints, empty)
{
    std::vector<gmt::vec2> x;
    const auto count = gmt::mergePoints(gmt::make_span(x.begin(), x.end()));
    EXPECT_EQ(count, 0);
}

TEST(MergePoints, exactMerge)
{
    std::vector<gmt::vec2> x{ { 1.0f, 1.0f }, { 1.0f, 1.0f } };
    const auto count = gmt::mergePoints(gmt::make_span(x.begin(), x.end()));
    EXPECT_EQ(count, 1);
}

TEST(MergePoints, notExactMerge)
{
    std::vector<gmt::vec2> x{ { 1.0f, 1.0f }, { 1.05f, 1.0f }, { 1.05f, 1.0001f } };
    const auto count = gmt::mergePoints(gmt::make_span(x.begin(), x.end()), 0.01f);
    EXPECT_EQ(count, 2);
    EXPECT_EQ(x[0].x, 1.0f);
    EXPECT_EQ(x[0].y, 1.0f);

    EXPECT_EQ(x[1].x, 1.05f);
    EXPECT_EQ(x[1].y, 1.0f);
}

TEST(CircleRectangleIntersection, noIntersection)
{
    std::vector<gmt::vec2> x;
    const auto result = gmt::circle::intersectsRectangle(0.5f, gmt::vec2(1.0f, -1.0f), 
        gmt::vec2(0.01f, 0.01f), std::back_inserter(x));
    EXPECT_FALSE(result);
}

TEST(CircleRectangleIntersection, simple)
{
    std::vector<gmt::vec2> x;
    const auto result = gmt::circle::intersectsRectangle(1.0f, gmt::vec2(2.0f, 0.0f),
        gmt::vec2(1.5f, 10.0f), std::back_inserter(x));
    EXPECT_TRUE(result);
    EXPECT_EQ(2, x.size());
    EXPECT_NEAR(x[0].x, x[1].x, 0.001f);
}

TEST(CircleRectangleIntersection, simple2)
{
    std::vector<gmt::vec2> x;
    const auto result = gmt::circle::intersectsRectangle(1.0f, gmt::vec2(2.0f, 2.0f),
        gmt::vec2(2.0f, 2.0f), std::back_inserter(x));
    EXPECT_TRUE(result);
    EXPECT_EQ(2, x.size());
    EXPECT_NEAR(1.0f, x[0].x, 0.001f);
    EXPECT_NEAR(0.0f, x[0].y, 0.001f);

    EXPECT_NEAR(0.0f, x[1].x, 0.001f);
    EXPECT_NEAR(1.0f, x[1].y, 0.001f);
}

TEST(CircleRectangleIntersection, cornerIntersection)
{
    std::vector<gmt::vec2> x;
    const auto result = gmt::circle::intersectsRectangle(1.0f, gmt::vec2(0.5f, 0.5f),
        gmt::vec2(0.5f, 0.5f), std::back_inserter(x));
    EXPECT_TRUE(result);
    EXPECT_EQ(2, x.size());
    EXPECT_NEAR(1.0f, x[0].x, 0.001f);
    EXPECT_NEAR(0.0f, x[0].y, 0.001f);

    EXPECT_NEAR(0.0f, x[1].x, 0.001f);
    EXPECT_NEAR(1.0f, x[1].y, 0.001f);
}

TEST(CircleRectangleIntersection, singleCorner)
{
    std::vector<gmt::vec2> x;
    const auto result = gmt::circle::intersectsRectangle(1.0f, gmt::vec2(2.0f, -1.0f),
        gmt::vec2(1.0f, 1.0f), std::back_inserter(x));
    EXPECT_TRUE(result);
    EXPECT_EQ(1, x.size());
    EXPECT_NEAR(1.0f, x[0].x, 0.001f);
    EXPECT_NEAR(0.0f, x[0].y, 0.001f);
}

TEST(CircleRectangleIntersection, circleContainsRectange)
{
    std::vector<gmt::vec2> x;
    const auto result = gmt::circle::intersectsRectangle(1.0f, gmt::vec2(0.0f, 0.0f),
        gmt::vec2(0.1f, 0.1f), std::back_inserter(x));
    EXPECT_TRUE(result);
    EXPECT_EQ(0, x.size());
}

TEST(CircleRectangleIntersection, orientedIntersection)
{
    std::vector<gmt::vec2> x;
    const auto result = gmt::circle::intersectsRectangle(1.0f, gmt::vec2(0.0f, -1.0f),
        gmt::vec2(0.5f, 10.0f), -glm::half_pi<float>(), std::back_inserter(x));
    EXPECT_TRUE(result);
    EXPECT_EQ(2, x.size());
    EXPECT_NEAR(x[0].y, x[1].y, 0.001f);
    EXPECT_NEAR(-x[0].x, x[1].x, 0.001f);
}

TEST(CircleRectangleIntersection, orientedIntersection2)
{
    std::vector<gmt::vec2> x;
    const auto t = sqrt(2.0f) / 2.0f;
    const auto result = gmt::circle::intersectsRectangle(1.0f, gmt::vec2(t, -t),
        gmt::vec2(1.0f, 10.0f), -glm::quarter_pi<float>(), std::back_inserter(x));
    EXPECT_TRUE(result);
    EXPECT_EQ(2, x.size());
    EXPECT_NEAR(x[0].x, x[0].y, 0.001f);
    EXPECT_NEAR(x[1].x, x[1].y, 0.001f);
    EXPECT_NEAR(x[0].x, -x[1].y, 0.001f);
    EXPECT_NEAR(t, x[0].x, 0.001f);
}