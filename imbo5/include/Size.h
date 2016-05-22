#pragma once

#include "Point.h"
#include <cstdlib>

namespace imbo5 {

/*!
 *  Represent the size of an N-dimensional rectangle
 */
template<unsigned int N>
class Size
{
public:

    using Dimension = std::size_t;

    Size(unsigned int s1, unsigned int s2)
    : _point(s1, s2)
    {}

    Size(unsigned int s1, unsigned int s2, unsigned int s3)
    : _point(s1, s2, s3)
    {}

    template<unsigned int M>
    const Dimension& at() const
    {
        return _point.at<M>();
    }

    template<unsigned int M>
    Dimension& at()
    {
        return _point.at<M>();
    }

    COMPONENT_ACCESSORS(Dimension, x, 0)
    COMPONENT_ACCESSORS(Dimension, y, 1)
    COMPONENT_ACCESSORS(Dimension, z, 2)

private:

    Point<unsigned int, N> _point; /*<! Stores the dimensions */
};

}
