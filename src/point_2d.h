#ifndef POINT_2D_H
#define POINT_2D_H

template <typename T>
struct Point2D
{
    Point2D(T _x = 0, T _y = 0)
        : x(_x), y(_y) {}

    T x;
    T y;
};

using Point2D_int = Point2D<int>;
using Point2D_float = Point2D<float>;

#endif // POINT_2D_H
