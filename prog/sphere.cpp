#include "sphere.hpp"

// Находим пересечения со сферой (отсутствует, либо одно при касании, либо 2)
std::tuple<QVector3D, QVector3D, double> Sphere::getIntersectionInfo(QVector3D start_point, QVector3D direction, double min_dist, double max_dist)
{
    QVector3D C = center;
    double r = radius;
    QVector3D OC = start_point - C;
 
    double k1 = scalarProduct(direction, direction);
    double k2 = 2 * scalarProduct(OC, direction);
    double k3 = scalarProduct(OC, OC) - r * r;

    double discriminant = k2 * k2 - 4 * k1 * k3;

    if (discriminant < 0)
        return { start_point, start_point, max_dist } ;

    double t1 = (-k2 + sqrt(discriminant)) / (2 * k1);
    double t2 = (-k2 - sqrt(discriminant)) / (2 * k1);

    double t;

    if ((t1 <= min_dist || t1 >= max_dist) && (t2 <= min_dist || t2 >= max_dist))
        return { start_point, start_point, max_dist };
    else if (t1 <= min_dist || t1 >= max_dist)
        t = t2;
    else if (t2 <= min_dist || t2 >= max_dist)
        t = t1;
    else
        t = std::min(t1, t2);

    QVector3D intersection = start_point + direction * t;

    QVector3D normal = (intersection - C).normalized();

    return { intersection, normal, t };
}

std::vector<std::shared_ptr<Object>> Sphere::split()
{
    std::vector<std::shared_ptr<Object>> f;
    Sphere *ptr = new Sphere((*this));
    f.push_back(std::shared_ptr<Sphere>(ptr));

    return f;
}


void Sphere::move(double x, double y, double z)
{
    center += QVector3D(x, y, z);
}


void Sphere::scale(double n)
{
    radius *= n;
}


void Sphere::rotate(double x, double y, double z)
{
    UNUSED(x);
    UNUSED(y);
    UNUSED(z);
}
