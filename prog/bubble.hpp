#ifndef BUBBLE_H
#define BUBBLE_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <QVector3D>
#include <QColor>

#include <cmath>

#include "object.hpp"

#define UNUSED(x) (void)(x)

class Bubble : public Object
{
    public:
        Bubble() = default;
        ~Bubble() = default;

        Bubble(std::shared_ptr<Object> newOuter, std::shared_ptr<Object> newInner)
        : outer(newOuter)
        , inner(newInner)
        { center = newOuter->getCenter(); };

        std::shared_ptr<Object> getOuter() { return outer; };
        void setOuter(std::shared_ptr<Object> newOuter) { outer = newOuter; };

        std::shared_ptr<Object> getInner() { return inner; };
        void setInner(std::shared_ptr<Object> newInner) { inner = newInner; };

        QVector3D getCenter() { return center; };
        void setCenter(QVector3D newCenter) { center = newCenter; }

        Substance getSubstance() { return outer->getSubstance(); };
        void setSubstance(QColor color, Coefficients coefficients, double specularIndex, double refractiveIndex, double dispersion)
                        { outer->setSubstance(color, coefficients, specularIndex, refractiveIndex, dispersion); };

        void setColor(QColor newColor) { outer->setColor(newColor); };
        void setCoefficients(Coefficients newCoefficients) { outer->setCoefficients(newCoefficients); };
        void setSpecularIndex(double newSpecularIndex) { outer->setSpecularIndex(newSpecularIndex); };
        void setRefractiveIndex(double newRefractiveIndex) { outer->setRefractiveIndex(newRefractiveIndex); };
        void setDispersion(double newDispersion) { outer->setDispersion(newDispersion); };

        QColor getColor() { return outer->getColor(); };
        Coefficients getCoefficients() { return outer->getCoefficients(); };
        double getSpecularIndex() { return outer->getSpecularIndex(); };
        double getRefractiveIndex() { return outer->getRefractiveIndex(); };
        double getDispersion() { return outer->getDispersion(); };

        double getThickness() {return thickness; };
        void setThickness(double newThickness) { thickness = newThickness; };

        std::tuple<QVector3D, QVector3D, double> getIntersectionInfo(QVector3D start_point, QVector3D direction, double min_dist, double max_dist);

        std::vector<std::shared_ptr<Object>> split() { return std::vector<std::shared_ptr<Object>>({outer, inner}); };

        void move(double x, double y, double z);
        void scale(double n);
        void rotate(double x, double y, double z);

    private:
        std::shared_ptr<Object> outer;
        std::shared_ptr<Object> inner;
        QVector3D center;
        double thickness = 0;
};

#endif //BUBBLE_H
