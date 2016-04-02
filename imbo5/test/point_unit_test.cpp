#include "Point.h"
#include <gtest/gtest.h>

#define CHECK_COMPONENT(point, component, expected_value) \
ASSERT_EQ(point.component(), expected_value) << "Check " #expected_value " component"; \

TEST(Point, Accessors)
{
    imbo5::Point<int, 3> p(1, 2, 3);

    CHECK_COMPONENT(p, x, 1);
    CHECK_COMPONENT(p, y, 2);
    CHECK_COMPONENT(p, z, 3);

    p.x() = 12;

    CHECK_COMPONENT(p, x, 12);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
