#include "Domain.hpp"
#include "Point2d_def.hpp"

int main(int argc, char **argv)
{
    using P2d = Point2d<double>;
    Domain domain;
    domain.vertices =
        {
            P2d(-10.0, -1.0),
            P2d(1.0, -1.0),
            P2d(3.0, -4.0),
            P2d(6.0, -4.0),
            P2d(10.0, 0.0),
            P2d(6.0, 4.0),
            P2d(3.0, 4.0),
            P2d(1.0, 1.0),
            P2d(-10.0, 1.0)
        };
    
    domain.save("domain.json");
}
