#include "Mesh.h"
#include <gtest/gtest.h>
#include <cstdlib>

using imbo5::Mesh;

namespace
{
    struct MeshInitialiser
    {
        int operator()(Mesh<int>::Index x, Mesh<int>::Index y) const
        {
            // unique for (x, y) if x and y are < 0xFFFF
            return (x << 16) | y;
        }
    };
}

TEST(Mesh, Construction)
{
    // Construct a rectangulat mesh and assign a unique element to each cell. Check that the mesh
    // is correctly initialised
    MeshInitialiser mi;
    imbo5::Size<2> size(5, 7);

    Mesh<int> m(size, mi);

    Mesh<int>::CellLabel cl(0, 0);

    for (std::size_t x = 0; x < size.x(); ++x)
    {
        cl.x() = x;
        for (std::size_t y = 0; y < size.y(); ++y)
        {
            cl.y() = y;
            auto actual = m[cl];
            auto expected = mi(x, y);
            EXPECT_EQ(expected, actual);
        }
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
