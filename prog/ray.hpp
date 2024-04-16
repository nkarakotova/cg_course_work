#ifndef RAY_H
#define RAY_H

#include <QVector3D>
#include <QColor>

struct Ray {
    QVector3D start_point;
    QVector3D direction;

    QColor color;
    double len;
    double n = 1.0; // показатель преломления среды
};

#endif // RAY_H
