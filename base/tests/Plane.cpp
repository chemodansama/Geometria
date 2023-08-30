#include <gtest/gtest.h>

#include <gmt/math/Plane.h>

namespace gmt
{

namespace tests
{

namespace plane
{

TEST(Plane, Simple)
{
    gmt::Plane p{
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 1.0f, 0.0f, 0.0f },
        glm::vec3{ 0.0f, 1.0f, 0.0f }
    };
    float distance;
    auto calculated = p.calculateSignedDistance(glm::vec3{}, &distance);
    EXPECT_EQ(calculated, true);
    EXPECT_NEAR(distance, 0.0f, 0.00001f);

    calculated = p.calculateSignedDistance(glm::vec3{ 1.0f, 0.0f, 0.0f }, &distance);
    EXPECT_EQ(calculated, true);
    EXPECT_NEAR(distance, 0.0f, 0.00001f);

    calculated = p.calculateSignedDistance(glm::vec3{ 100.0f, 0.0f, 0.0f }, &distance);
    EXPECT_EQ(calculated, true);
    EXPECT_NEAR(distance, 0.0f, 0.00001f);

    calculated = p.calculateSignedDistance(glm::vec3{ 100.0f, 100.0f, 0.0f }, &distance);
    EXPECT_EQ(calculated, true);
    EXPECT_NEAR(distance, 0.0f, 0.00001f);

    calculated = p.calculateSignedDistance(glm::vec3{ 0.0f, 100.0f, 0.0f }, &distance);
    EXPECT_EQ(calculated, true);
    EXPECT_NEAR(distance, 0.0f, 0.00001f);

    calculated = p.calculateSignedDistance(glm::vec3{ 0.0f, 100.0f, 10.0f }, &distance);
    EXPECT_EQ(calculated, true);
    EXPECT_NEAR(distance, 10.0f, 0.00001f);
}

TEST(Plane, CantCalculate)
{
    gmt::Plane p{
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 0.0f, 1.0f, 0.0f }
    };

    float distance;
    auto calculated = p.calculateSignedDistance(glm::vec3{}, &distance);
    EXPECT_EQ(calculated, false);
}

TEST(Plane, NegativeDistance)
{
    gmt::Plane p{
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 0.0f, 1.0f, 0.0f },
        glm::vec3{ 0.0f, 0.0f, 1.0f }
    };

    float distance;
    auto calculated = p.calculateSignedDistance(glm::vec3{ -1.0, 0.0f, 0.0f }, &distance);
    EXPECT_EQ(calculated, true);
    EXPECT_NEAR(distance, -1.0f, 0.00001f);

    calculated = p.calculateSignedDistance(glm::vec3{ -15.0, 17.0f, 37.0f }, &distance);
    EXPECT_EQ(calculated, true);
    EXPECT_NEAR(distance, -15.0f, 0.00001f);
}

TEST(Plane, NonOrthogonalUV)
{
    gmt::Plane p{
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 1.0f, 0.0f, 0.0f },
        glm::vec3{ 1.0f, 1.0f, 0.0f }
    };

    float distance;
    auto calculated = p.calculateSignedDistance(glm::vec3{ -1.0, 0.0f, 0.0f }, &distance);
    EXPECT_EQ(calculated, true);
    EXPECT_NEAR(distance, 0.0f, 0.00001f);

    calculated = p.calculateSignedDistance(glm::vec3{ -15.0, 17.0f, 37.0f }, &distance);
    EXPECT_EQ(calculated, true);
    EXPECT_NEAR(distance, 37.0f, 0.00001f);
}

TEST(Plane, NonZeroOrigin)
{
    gmt::Plane p{ 
        glm::vec3{ 10.0f, 15.0f, 20.0f },
        glm::vec3{ 11.0f, 15.0f, 20.0f },
        glm::vec3{ 11.0f, 16.0f, 20.0f }
    };

    float distance;
    auto calculated = p.calculateSignedDistance(glm::vec3{ 10.0, 17.0f, 37.0f }, &distance);
    EXPECT_EQ(calculated, true);
    EXPECT_NEAR(distance, 17.0f, 0.00001f);
}

}

}

}
