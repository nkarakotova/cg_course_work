#ifndef SCENE_H
#define SCENE_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <math.h>
#include <tuple>
#include <stack>
#include <thread>

#include <QImage>
#include <QVector3D>
#include <QVector4D>

#include "light.hpp"
#include "object.hpp"
#include "triangle.hpp"
#include "loader.hpp"
#include "ray.hpp"

#define THREADS_COUNT 64

#define NOTHING   0
#define SOMETHING 1

#define EPS 1e-7

#define TRACE_DEPTH 10
#define RAYS        1
#define SCREEN_Z    1000

#define MIN_DIST 0.1
#define MAX_DIST 100'000'000'000'000'000

#define MIN_DIST_LIGHT 0.0001
#define MAX_DIST_LIGHT 1

#define OUT 0
#define IN 1

class Scene
{
    public:
        Scene() = default;
        Scene(int width, int height) { canvasWidth = width; canvasHeight = height; };
        Scene &operator = (const Scene &scene) = default;

        void setLoader(Loader newLoader) { sceneLoader = newLoader; };
        std::tuple<int, int, std::vector<std::string>> loadScene(std::string filename);

        void addLight(Light newLight) { lights.push_back(newLight); };

        void initializedImage() { image = std::make_shared<QImage>(canvasWidth, canvasHeight, QImage::Format_RGB32); };
        std::shared_ptr<QImage> getImage() { return image; };

        QVector3D getReflectVector(QVector3D viewVector, QVector3D normal);
        QVector3D getRefractVector(QVector3D viewVector, QVector3D normal, double r, double i, bool where);

        void findIntensity(QVector3D point, QVector3D normal, QVector3D direction, double &diffuseIntensity, double &mirrorIntensity);
        QColor findNewColor(QVector3D point, QVector3D normal, QVector3D direction, QColor color);

        QColor getColor(double intensity, double specIntensity, Coefficients coefficients, QColor color, QColor reflectColor, QColor refractColor, double reflectK, int depth);
        QColor getColorUsingCoefficient(QColor color, QColor reflect_color, QColor refract_color, double r, double f);

        std::tuple<int, int, QVector3D, QVector3D> findClosestIntersection(QVector3D start_point, QVector3D direction, double min_dist, double max_dist);
        double findLightIntensity(QVector3D start_point, QVector3D direction, double min_dist, double max_dist);
        QColor TraceRay(Ray ray, double min_dist, double max_dist, int trace_depth, bool where);

        void drawThread(int x_start, int x_end, int y_start, int y_end, int trace_depth);
        void draw(int trace_depth = TRACE_DEPTH);

        void moveObject(int index, double x, double y, double z) { whole_objects[index]->move(x, y, z); };
        void scaleObject(int index, double n) { whole_objects[index]->scale(n); };
        void rotateObject(int index, double x, double y, double z) { whole_objects[index]->rotate(x, y, z); };

        void changeObjectColor(int index, QColor newColor) { whole_objects[index]->setColor(newColor); };

        void changeCoefficients(int index, Coefficients newCoefficients) { whole_objects[index]->setCoefficients(newCoefficients); };
        void changeSpecularIndex(int index, double newSpecularIndex) { whole_objects[index]->setSpecularIndex(newSpecularIndex); };
        void changeRefractiveIndex(int index, double newRefractiveIndex) { whole_objects[index]->setRefractiveIndex(newRefractiveIndex); };
        void changeDispersion(int index, double newDispersion) { whole_objects[index]->setDispersion(newDispersion); };

        QColor getObjectColor(int index) { return whole_objects[index]->getColor(); };

        Coefficients getCoefficients(int index) { return whole_objects[index]->getCoefficients(); };
        double getSpecularIndex(int index) { return whole_objects[index]->getSpecularIndex(); };
        double getRefractiveIndex(int index) { return whole_objects[index]->getRefractiveIndex(); };
        double getDispersion(int index) { return whole_objects[index]->getDispersion(); };

        void setLightPosition(int index, QVector3D newPosition) { lights[index].setView(newPosition); };
        void setIntensity(int index, double newIntensity) { lights[index].setIntensity(newIntensity); };

        QVector3D getLightPosition(int index) { return lights[index].getView(); };
        double getIntensity(int index) { return lights[index].getIntensity(); };

        void delLightByIndex(int index) { lights.erase(lights.begin() + index); };
        void addLight(QVector3D newPosition, double newIntensity) { lights.push_back(Light(newPosition, newIntensity)); };


    private:
        Loader sceneLoader;

        int canvasWidth = 700;
        int canvasHeight = 700;

        int max_trace_depth;

        QVector3D eyePosition = QVector3D(450, 250, 3000);

        QColor lights_color = QColor(255, 255, 255);

        std::shared_ptr<QImage> image;

        std::vector<std::shared_ptr<Object>> objects;
        std::vector<std::shared_ptr<Object>> whole_objects;
        std::vector<Light> lights;
};


#endif //SCENE_H
