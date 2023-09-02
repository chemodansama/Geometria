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

    EXPECT_EQ((bool)p, true);

    auto distance = p.calculateSignedDistance(glm::vec3{});
    EXPECT_NEAR(distance, 0.0f, 0.00001f);

    distance = p.calculateSignedDistance(glm::vec3{ 1.0f, 0.0f, 0.0f });
    EXPECT_NEAR(distance, 0.0f, 0.00001f);

    distance = p.calculateSignedDistance(glm::vec3{ 100.0f, 0.0f, 0.0f });
    EXPECT_NEAR(distance, 0.0f, 0.00001f);

    distance = p.calculateSignedDistance(glm::vec3{ 100.0f, 100.0f, 0.0f });
    EXPECT_NEAR(distance, 0.0f, 0.00001f);

    distance = p.calculateSignedDistance(glm::vec3{ 0.0f, 100.0f, 0.0f });
    EXPECT_NEAR(distance, 0.0f, 0.00001f);

    distance = p.calculateSignedDistance(glm::vec3{ 0.0f, 100.0f, 10.0f });
    EXPECT_NEAR(distance, 10.0f, 0.00001f);
}

TEST(Plane, CantCalculate)
{
    gmt::Plane p{
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 0.0f, 1.0f, 0.0f }
    };
    EXPECT_EQ((bool)p, false);
}

TEST(Plane, NegativeDistance)
{
    gmt::Plane p{
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 0.0f, 1.0f, 0.0f },
        glm::vec3{ 0.0f, 0.0f, 1.0f }
    };
    EXPECT_EQ((bool)p, true);
    auto distance = p.calculateSignedDistance(glm::vec3{ -1.0, 0.0f, 0.0f });
    EXPECT_NEAR(distance, -1.0f, 0.00001f);

    distance = p.calculateSignedDistance(glm::vec3{ -15.0, 17.0f, 37.0f });
    EXPECT_NEAR(distance, -15.0f, 0.00001f);
}

TEST(Plane, NonOrthogonalUV)
{
    gmt::Plane p{
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 1.0f, 0.0f, 0.0f },
        glm::vec3{ 1.0f, 1.0f, 0.0f }
    };
    EXPECT_EQ((bool)p, true);

    auto distance = p.calculateSignedDistance(glm::vec3{ -1.0, 0.0f, 0.0f });
    EXPECT_NEAR(distance, 0.0f, 0.00001f);

    distance = p.calculateSignedDistance(glm::vec3{ -15.0, 17.0f, 37.0f });
    EXPECT_NEAR(distance, 37.0f, 0.00001f);
}

TEST(Plane, NonZeroOrigin)
{
    gmt::Plane p{ 
        glm::vec3{ 10.0f, 15.0f, 20.0f },
        glm::vec3{ 11.0f, 15.0f, 20.0f },
        glm::vec3{ 11.0f, 16.0f, 20.0f }
    };
    EXPECT_EQ((bool)p, true);

    auto distance = p.calculateSignedDistance(glm::vec3{ 10.0, 17.0f, 37.0f });
    EXPECT_NEAR(distance, 17.0f, 0.00001f);
}

TEST(Plane, Project0)
{
    gmt::Plane p{ 
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 1.0f, 0.0f, 0.0f },
        glm::vec3{ 0.0f, 0.0f, 1.0f }
    };
    EXPECT_EQ((bool)p, true);

    auto projected = p.project(glm::vec3{ 1.0, 2.0f, 3.0f });
    EXPECT_NEAR(projected.x, 1.0f, 0.00001f);
    EXPECT_NEAR(projected.y, 0.0f, 0.00001f);
    EXPECT_NEAR(projected.z, 3.0f, 0.00001f);

    projected = p.project(glm::vec3{ 1.0, 22.0f, 3.0f });
    EXPECT_NEAR(projected.x, 1.0f, 0.00001f);
    EXPECT_NEAR(projected.y, 0.0f, 0.00001f);
    EXPECT_NEAR(projected.z, 3.0f, 0.00001f);
}

TEST(Plane, Project1)
{
    gmt::Plane p{ 
        glm::vec3{ 10.0f, 10.0f, 0.0f },
        glm::vec3{ -10.0f, -10.0f, 0.0f },
        glm::vec3{ 10.0f, 10.0f, 10.0f }
    };
    EXPECT_EQ((bool)p, true);

    auto projected = p.project(glm::vec3{ 2.0, 0.0f, 3.0f });
    EXPECT_NEAR(projected.x, 1.0f, 0.00001f);
    EXPECT_NEAR(projected.y, 1.0f, 0.00001f);
    EXPECT_NEAR(projected.z, 3.0f, 0.00001f);
}

TEST(Plane, Intersect0)
{
    gmt::Plane p{ 
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 1.0f, 0.0f, 0.0f },
        glm::vec3{ 0.0f, 0.0f, 1.0f }
    };
    EXPECT_EQ((bool)p, true);

    auto i = p.intersect(glm::vec3{ 2.0, 2.0f, 0.0f }, glm::vec3{ -2.0, -2.0f, 0.0f });
    EXPECT_NEAR(i.x, 0.0f, 0.00001f);
    EXPECT_NEAR(i.y, 0.0f, 0.00001f);
    EXPECT_NEAR(i.z, 0.0f, 0.00001f);

    i = p.intersect(glm::vec3{ 2.0, 2.0f, 30.0f }, glm::vec3{ -2.0, -2.0f, 30.0f });
    EXPECT_NEAR(i.x, 0.0f, 0.00001f);
    EXPECT_NEAR(i.y, 0.0f, 0.00001f);
    EXPECT_NEAR(i.z, 30.0f, 0.00001f);

    i = p.intersect(glm::vec3{ 3.0, 3.0f, 30.0f }, glm::vec3{ 1.0, 1.0f, 30.0f });
    EXPECT_NEAR(i.x, 0.0f, 0.00001f);
    EXPECT_NEAR(i.y, 0.0f, 0.00001f);
    EXPECT_NEAR(i.z, 30.0f, 0.00001f);

    i = p.intersect(glm::vec3{ -3.0, -3.0f, 30.0f }, glm::vec3{ -1.0, -1.0f, 30.0f });
    EXPECT_NEAR(i.x, 0.0f, 0.00001f);
    EXPECT_NEAR(i.y, 0.0f, 0.00001f);
    EXPECT_NEAR(i.z, 30.0f, 0.00001f);

    i = p.intersect(glm::vec3{ -1.0, -1.0f, 30.0f }, glm::vec3{ -3.0, -3.0f, 30.0f });
    EXPECT_NEAR(i.x, 0.0f, 0.00001f);
    EXPECT_NEAR(i.y, 0.0f, 0.00001f);
    EXPECT_NEAR(i.z, 30.0f, 0.00001f);
}

TEST(Plane, Intersect1)
{
    gmt::Plane p{ 
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 4.0f, 1.0f, 0.0f },
        glm::vec3{ 4.0f, 1.0f, 1.0f }
    };
    EXPECT_EQ((bool)p, true);

    auto i = p.intersect(glm::vec3{ 3.0, 0.0f, 0.0f }, glm::vec3{ 1.0, 1.0f, 0.0f });
    EXPECT_NEAR(i.x, 2.0f, 0.00001f);
    EXPECT_NEAR(i.y, 0.5f, 0.00001f);
    EXPECT_NEAR(i.z, 0.0f, 0.00001f);
}

}

}

}
