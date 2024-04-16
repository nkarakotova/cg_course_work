#include "bubble.hpp"
#include "my_math.hpp"

std::tuple<QVector3D, QVector3D, double> Bubble::getIntersectionInfo(QVector3D start_point, QVector3D direction, double min_dist, double max_dist)
{
    UNUSED(start_point);
    UNUSED(direction);
    UNUSED(min_dist);
    UNUSED(max_dist);

    return std::tuple<QVector3D, QVector3D, double> (QVector3D(0,0,0), QVector3D(0,0,0), 0.0);
}

void Bubble::move(double x, double y, double z)
{
    outer->move(x, y, z);
    inner->move(x, y, z);
}

void Bubble::scale(double n)
{
    outer->scale(n);
    inner->scale(n);
}

void Bubble::rotate(double x, double y, double z)
{
    UNUSED(x);
    UNUSED(y);
    UNUSED(z);
}
