#include "my_math.hpp"

double scalarProduct(QVector3D v1, QVector3D v2) { return QVector3D::dotProduct(v1, v2); }
QVector3D vectorProduct(QVector3D v1, QVector3D v2) { return QVector3D::crossProduct(v1, v2); }
