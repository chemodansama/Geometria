#include <gtest/gtest.h>

#include <gmt/path.h>

namespace gmt
{

namespace tests
{

namespace path
{

TEST(Path, CollapseSingle)
{
    const auto result = gmt::path::collapse("C:/hello/../world");
    EXPECT_EQ(result, "C:/world");
}

TEST(Path, CollapseMultiple)
{
    const auto result = gmt::path::collapse("C:/hello/../world/../foo/bar.txt");
    EXPECT_EQ(result, "C:/foo/bar.txt");
}

TEST(Path, CollapseMultiple2)
{
    const auto result = gmt::path::collapse("C:/hello/../world/../../bar.txt");
    EXPECT_EQ(result, "bar.txt");
}

TEST(Path, EmptyResult)
{
    const auto result = gmt::path::collapse("C:/../");
    EXPECT_EQ(result, "");
}

TEST(Path, EmptyInput)
{
    const auto result = gmt::path::collapse("");
    EXPECT_EQ(result, "");
}

TEST(Path, OverlyCollapse)
{
    const auto result = gmt::path::collapse("C:/../../../");
    EXPECT_EQ(result, "");
}

}

}

}