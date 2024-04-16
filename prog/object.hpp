#ifndef OBJECT_H
#define OBJECT_H

#include <tuple>

#include <QVector3D>
#include <QColor>
#include <memory>

#include "substance.hpp"

#define BUBBLE 1;

class Triangle;

class Object
{
    public:
        Object() = default;
        virtual ~Object() = default;

        virtual Substance getSubstance() { return substance; };
        virtual void setSubstance(QColor color, Coefficients coefficients, double specularIndex, double refractiveIndex, double dispersion)
                                { substance = Substance(color, coefficients, specularIndex, refractiveIndex, dispersion); };

        virtual void setColor(QColor newColor) { substance.setColor(newColor); };
        virtual void setCoefficients(Coefficients newCoefficients) { substance.setCoefficients(newCoefficients); };
        virtual void setSpecularIndex(double newSpecularIndex) { substance.setSpecularIndex(newSpecularIndex); };
        virtual void setRefractiveIndex(double newRefractiveIndex) { substance.setRefractiveIndex(newRefractiveIndex); };
        virtual void setDispersion(double newDispersion) { substance.setDispersion(newDispersion); };

        virtual QColor getColor() { return substance.getColor(); };
        virtual Coefficients getCoefficients() { return substance.getCoefficients(); };
        virtual double getSpecularIndex() { return substance.getSpecularIndex(); };
        virtual double getRefractiveIndex() { return substance.getRefractiveIndex(); };
        virtual double getDispersion() { return substance.getDispersion(); };

        virtual QVector3D getCenter() = 0;
        virtual void setCenter(QVector3D newCenter) = 0;

        virtual double getThickness() = 0;
        virtual void setThickness(double newThickness) = 0;

        virtual std::tuple<QVector3D, QVector3D, double> getIntersectionInfo(QVector3D start_point, QVector3D direction, double min_dist, double max_dist) = 0;

        virtual std::vector<std::shared_ptr<Object>> split() = 0;

        virtual void move(double x, double y, double z) = 0;
        virtual void scale(double n) = 0;
        virtual void rotate(double x, double y, double z) = 0;
    
    private:
        Substance substance;
        QVector3D center;
//        double thickness = 0;
};

#endif //OBJECT_H
