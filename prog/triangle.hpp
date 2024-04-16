#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QColor>

#include <cmath>

#include "my_math.hpp"

class Triangle
{
    public:
        Triangle() = default;

        Triangle(QVector3D *newPointA,
                 QVector3D *newPointB,
                 QVector3D *newPointC)
                    :pointA(newPointA),
                     pointB(newPointB),
                     pointC(newPointC) {};

        QVector3D* getA() { return pointA; };
        QVector3D* getB() { return pointB; };
        QVector3D* getC() { return pointC; };

        void checkNormal(QVector3D pointInside);

        void findNormal(QVector3D pointInside);
        QVector3D getNormal() { return normal; };
        void setNormal(QVector3D new_normal) { normal = new_normal; };

        bool isIntersection(QVector3D direction);
        std::tuple<QVector3D, double> findIntersection(QVector3D start_point, QVector3D direction);
        bool isInside(QVector3D point);

    private:
        QVector3D *pointA, *pointB, *pointC;
        QVector3D normal;
};


#endif //TRIANGLE_H
