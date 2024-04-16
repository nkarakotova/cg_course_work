#ifndef SUBSTANCE_H
#define SUBSTANCE_H

#include <vector>

#include <QColor>
#include <QVector4D>

#define RC 0.000000702
#define GC 0.000000542
#define BC 0.000000475

struct Coefficients {
    double ambient;
    double diffuse;
    double specular;
    double refract;
};

class Substance
{
public:
    Substance() = default;
    Substance(QColor newColor, Coefficients newCoefficients, double newSpecularIndex, double newRefractiveIndex, double newDispersion);
    Substance(Substance& substance);

    Substance& operator=(Substance& substance);
    Substance& operator=(Substance&& substance);

    QColor getColor() { return color; };
    void setColor(QColor newColor) { color = newColor; };

    Coefficients getCoefficients() { return coefficients; };
    double getSpecularIndex() { return specularIndex; };
    std::vector<double> getRefractiveIndices() {return refractiveIndices; };
    double getDispersion() { return dispersion; };

    void setCoefficients(Coefficients newCoefficients) { coefficients = newCoefficients; };
    void setSpecularIndex(double newSpecularIndex) { specularIndex = newSpecularIndex; };
    void setRefractiveIndices(double newRefractiveIndex, double newDispersion);
    void setDispersion(double newDispersion) { dispersion = newDispersion; }; // тут по идее тогда еще преломление надо менять

    void setRefractiveIndex(double newRefractiveIndex) { refractiveIndex = newRefractiveIndex; setRefractiveIndices(newRefractiveIndex, dispersion); };
    double getRefractiveIndex() { return refractiveIndex; };

private:
    QColor color;

    Coefficients coefficients;

    double specularIndex; // Показатель отражения
    std::vector<double> refractiveIndices; // Показатели преломления по каждой компоненте
    double dispersion; // Дисперсия

    double refractiveIndex;
};

#endif // SUBSTANCE_H
