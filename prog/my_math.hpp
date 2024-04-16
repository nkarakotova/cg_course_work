#ifndef MY_MATH_H
#define MY_MATH_H

#include <QVector3D>

#define UNUSED(x) (void)(x)

#define EPS 1e-7

double scalarProduct(QVector3D v1, QVector3D v2);
QVector3D vectorProduct(QVector3D v1, QVector3D v2);

#endif // MY_MATH_H
