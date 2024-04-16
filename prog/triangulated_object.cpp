#include "triangulated_object.hpp"

std::tuple<QVector3D, QVector3D, double> TriangulatedObject::getIntersectionInfo(QVector3D start_point, QVector3D direction, double min_dist, double max_dist)
{
    double min_t = max_dist;

    QVector3D closest_point(0, 0, 0);
    QVector3D closest_normal(0, 0, 0);

    // intersection, normal, t
    auto [intersection, normal, t] = sphere.getIntersectionInfo(start_point, direction, min_dist, max_dist);

    if (t == max_dist)
        return { closest_point, closest_normal.normalized(), min_t };

    for (int i = 0; i < faces.size(); i++)
    {
        Triangle cur_face = faces[i];

        // Проверяем лицевая ли грань
        if (scalarProduct(cur_face.getNormal(), direction) >= 0) continue;

        // Проверяем, пересекает ли луч плоскость грани
        if (!cur_face.isIntersection(direction)) continue;

        // Ищем пересечение
        auto [point, t] = cur_face.findIntersection(start_point, direction);

        // Проверяем, находится ли пересечение на грани
        if (!cur_face.isInside(point)) continue;

        // Если пересечение является ближайшим к наблюдателю, то запоминаем его 
        // (не рассматриваем то, что перед экраном)
        if (t < min_t && t > min_dist && t < max_dist)
        {
            min_t = t;
            closest_point = point;
            closest_normal = cur_face.getNormal();
        }
    }

    return { closest_point, closest_normal.normalized(), min_t };
}

void TriangulatedObject::findSphere()
{
    double max_len = 0;
    double cur_len;

    for (auto &point : points)
    {
        cur_len = (point - center).length();

        if (cur_len > max_len)
            max_len = cur_len;
    }

    Sphere newSphere(center, max_len);

    sphere = newSphere;
}

std::vector<std::shared_ptr<Object>> TriangulatedObject::split()
{
    std::vector<std::shared_ptr<Object>> f;
    TriangulatedObject *ptr = new TriangulatedObject((*this));
    f.push_back(std::shared_ptr<TriangulatedObject>(ptr));

    return f;
}


void TriangulatedObject::move(double x, double y, double z)
{
    QVector3D diff = QVector3D(x, y, z);

    for (int i = 0; i < points.size(); i++)
        points[i] += diff;

    center += diff;

    sphere.setCenter(center);
}


void TriangulatedObject::scale(double n)
{
    for (int i = 0; i < points.size(); i++)
        points[i] = (points[i] - center) * n + center;

    sphere.setRadius(sphere.getRadius() * n);
}


void TriangulatedObject::rotate(double x, double y, double z)
{
    x *= M_PI / 180;
    y *= M_PI / 180;
    z *= M_PI / 180;

    double x_c = center.x();
    double y_c = center.y();
    double z_c = center.z();

    for (int i = 0; i < points.size(); i++)
    {
        double x_p = points[i].x();
        double y_p = points[i].y();
        double z_p = points[i].z();

        double translatedX = x_p - x_c;
        double translatedY = y_p - y_c;
        double translatedZ = z_p - z_c;

        // Поворот вокруг оси X
        double rotatedX = translatedX;
        double rotatedY = translatedY * cos(x) - translatedZ * sin(x);
        double rotatedZ = translatedY * sin(x) + translatedZ * cos(x);

        // Поворот вокруг оси Y
        float tempX = rotatedX * cos(y) + rotatedZ * sin(y);
        rotatedZ = -rotatedX * sin(y) + rotatedZ * cos(y);
        rotatedX = tempX;

        // Поворот вокруг оси Z
        tempX = rotatedX * cos(z) - rotatedY * sin(z);
        rotatedY = rotatedX * sin(z) + rotatedY * cos(z);
        rotatedX = tempX;

        points[i].setX(rotatedX + x_c);
        points[i].setY(rotatedY + y_c);
        points[i].setZ(rotatedZ + z_c);
    }

    for (int i = 0; i < faces.size(); i++)
        faces[i].findNormal(center);
}
