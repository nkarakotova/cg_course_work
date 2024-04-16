#include "substance.hpp"

Substance::Substance(QColor newColor, Coefficients newCoefficients, double newSpecularIndex, double newRefractiveIndex, double newDispersion)
    : color(newColor)
    , coefficients(newCoefficients)
    , specularIndex(newSpecularIndex)
    , dispersion(newDispersion)
    , refractiveIndex(newRefractiveIndex)
{
    setRefractiveIndices(newRefractiveIndex, newDispersion);
}

Substance::Substance(Substance& substance)
    : color(substance.color)
    , coefficients(substance.coefficients)
    , specularIndex(substance.specularIndex)
    , refractiveIndices(substance.refractiveIndices)
    , dispersion(substance.dispersion)
    , refractiveIndex(substance.refractiveIndex)
{
}

Substance& Substance::operator=(Substance& substance)
{
    color = substance.color;
    coefficients = substance.coefficients;
    refractiveIndices = substance.refractiveIndices;
    specularIndex = substance.specularIndex;
    dispersion = substance.dispersion;
    refractiveIndex = substance.refractiveIndex;
    return *this;
}

Substance& Substance::operator=(Substance&& substance)
{
    color = substance.color;
    coefficients = substance.coefficients;
    refractiveIndices = substance.refractiveIndices;
    specularIndex = substance.specularIndex;
    dispersion = substance.dispersion;
    refractiveIndex = substance.refractiveIndex;
    return *this;
}

void Substance::setRefractiveIndices(double newRefractiveIndex, double newDispersion)
{
    double C = newRefractiveIndex - newDispersion * RC;

    refractiveIndices.clear();
    refractiveIndices.push_back(newDispersion * RC + C);
    refractiveIndices.push_back(newDispersion * GC + C);
    refractiveIndices.push_back(newDispersion * BC + C);
}
