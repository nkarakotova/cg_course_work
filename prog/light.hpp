#ifndef LIGHT_H
#define LIGHT_H

#include <cmath>

#include <QVector3D>

class Light
{
    public:
        Light() = default;
        Light(QVector3D newView, double newIntensity) : view(newView), intensity(newIntensity) {};

        void setView(QVector3D newView) { view = newView; };
        void setIntensity(double newIntensity) { intensity = newIntensity; };


        QVector3D getView() { return view; };
        double getIntensity() { return intensity; };

    private:
        QVector3D view;
        double intensity;
};

#endif // LIGHT_H
