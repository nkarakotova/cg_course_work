#ifndef TRIANGULATED_OBJECT_H
#define TRIANGULATED_OBJECT_H

#include <tuple>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <QVector3D>
#include <QColor>

#include <cmath>

#include "object.hpp"
#include "triangle.hpp"
#include "sphere.hpp"

class TriangulatedObject : public Object
{
    public:
        TriangulatedObject() = default;
        ~TriangulatedObject() = default;

        std::vector<Triangle> getFaces() { return faces; };
        void setFaces(const std::vector<Triangle> &newFaces) { faces = newFaces; };

        void setPoints(const std::vector<QVector3D> &newPoints) { points = newPoints; findSphere(); };
        QVector3D* getPointAt(int index) { return &(points[index]); };

        QVector3D getCenter() { return center; };
        void setCenter(QVector3D newCenter) { center = newCenter; };

        double getThickness() {return thickness; };
        void setThickness(double newThickness) { thickness = newThickness; };

        std::tuple<QVector3D, QVector3D, double> getIntersectionInfo(QVector3D start_point, QVector3D direction, double min_dist, double max_dist);

        void findSphere();

        std::vector<std::shared_ptr<Object>> split();

        void move(double x, double y, double z);
        void scale(double n);
        void rotate(double x, double y, double z);

    private:
        std::vector<Triangle> faces;
        std::vector<QVector3D> points;
        QVector3D center;
        double thickness = 0;

        Sphere sphere;
};


#endif // TRIANGULATED_OBJECT_H
