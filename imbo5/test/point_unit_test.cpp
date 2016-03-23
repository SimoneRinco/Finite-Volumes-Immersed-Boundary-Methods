#include "Point.h"

int main(int argc, char **argv)
{
    imbo5::Point<int, 2> p2i(3, 5);
    bool ok = (p2i.at<0>() == 3);
    ok = ok && (p2i.at<1>() == 5);
    p2i.at<1>() = 6;
    ok = ok && (p2i.at<1>() == 6);
    return ok ? 0 : 1;
}
