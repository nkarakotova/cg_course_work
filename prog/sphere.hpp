#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <QVector3D>
#include <QColor>

#include <cmath>

#include "object.hpp"
#include "my_math.hpp"

class Sphere : public Object
{
    public:
        Sphere() = default;
        ~Sphere() = default;

        Sphere(QVector3D newCenter, double newRadius) : center(newCenter), radius(newRadius) {};
        Sphere& operator=(Sphere& new_sphere) { center = new_sphere.getCenter(); radius =  new_sphere.getRadius(); return *this; };

        QVector3D getCenter() { return center; };
        void setCenter(QVector3D newCenter) { center = newCenter; };

        double getRadius() { return radius; };
        void setRadius(double newRadius) { radius = newRadius; };

        double getThickness() {return thickness; };
        void setThickness(double newThickness) { thickness = newThickness; };

        std::tuple<QVector3D, QVector3D, double> getIntersectionInfo(QVector3D start_point, QVector3D direction, double min_dist, double max_dist);

        std::vector<std::shared_ptr<Object>> split();

        void move(double x, double y, double z);
        void scale(double n);
        void rotate(double x, double y, double z);

    private:
        QVector3D center;
        double radius = 0;
        double thickness = 0;
};


#endif //SPHERE_H
