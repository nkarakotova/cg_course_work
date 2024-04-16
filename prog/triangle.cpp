#include "triangle.hpp"

#define EPS 1e-7

void Triangle::findNormal(QVector3D pointInside) 
{
    // Векторное произведение смежных сторон
    normal = vectorProduct(*pointC - *pointB, *pointA - *pointB).normalized();

    checkNormal(pointInside);
}

void Triangle::checkNormal(QVector3D pointInside)
{
    // Сравниваем положение нормали с внутренней точкой
    if (scalarProduct(pointInside - *pointB, normal) > 0)
        normal = -normal;
}

// Проверяем, есть ли пересечение
bool Triangle::isIntersection(QVector3D direction)
{
    return fabs(scalarProduct(normal, direction)) > EPS;
}

// Ищем пересечение
std::tuple<QVector3D, double> Triangle::findIntersection(QVector3D start_point, QVector3D direction)
{
    double t = scalarProduct(*pointA - start_point, normal) / scalarProduct(direction, normal);

    QVector3D intersection = start_point + direction * t;

    return { intersection, t };
}

// Проверяем, что пересечение на грани
bool Triangle::isInside(QVector3D point)
{
    QVector3D tmpPointA = *pointA;
    QVector3D tmpPointB = *pointB;
    QVector3D tmpPointC = *pointC;

    // Тк пересечение в плоскости грани, сводим задачу к 2х мерной

    // Если не параллельно оси z
    if (fabs(scalarProduct(normal, QVector3D(0, 0, 1))) > EPS)
    {
        tmpPointA.setZ(0);
        tmpPointB.setZ(0);
        tmpPointC.setZ(0);
        point.setZ(0);

        QVector3D v1 = vectorProduct(tmpPointB - tmpPointA, point - tmpPointA);
        QVector3D v2 = vectorProduct(tmpPointC - tmpPointB, point - tmpPointB);
        QVector3D v3 = vectorProduct(tmpPointA - tmpPointC, point - tmpPointC);

        if ((v1.z() >=0 && v2.z() >= 0 && v3.z() >= 0) ||
            (v1.z() <=0 && v2.z() <= 0 && v3.z() <= 0))
            return true;
    }
    // Если не параллельно оси y
    else if (fabs(scalarProduct(normal, QVector3D(0, 1, 0))) > EPS)
    {
        tmpPointA.setY(0);
        tmpPointB.setY(0);
        tmpPointC.setY(0);
        point.setY(0);

        QVector3D v1 = vectorProduct(tmpPointB - tmpPointA, point - tmpPointA);
        QVector3D v2 = vectorProduct(tmpPointC - tmpPointB, point - tmpPointB);
        QVector3D v3 = vectorProduct(tmpPointA - tmpPointC, point - tmpPointC);

        if ((v1.y() >=0 && v2.y() >= 0 && v3.y() >= 0) ||
            (v1.y() <=0 && v2.y() <= 0 && v3.y() <= 0))
            return true;
    }
    else
    {
        tmpPointA.setX(0);
        tmpPointB.setX(0);
        tmpPointC.setX(0);
        point.setX(0);

        QVector3D v1 = vectorProduct(tmpPointB - tmpPointA, point - tmpPointA);
        QVector3D v2 = vectorProduct(tmpPointC - tmpPointB, point - tmpPointB);
        QVector3D v3 = vectorProduct(tmpPointA - tmpPointC, point - tmpPointC);

        if ((v1.x() >=0 && v2.x() >= 0 && v3.x() >= 0) ||
            (v1.x() <=0 && v2.x() <= 0 && v3.x() <= 0))
            return true;
    }

    return false;

}
