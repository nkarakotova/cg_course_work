#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <tuple>

#include <QVector3D>
#include <QColor>

#include "triangle.hpp"
#include "sphere.hpp"
#include "bubble.hpp"
#include "triangulated_object.hpp"
#include "object.hpp"
#include "light.hpp"

class Loader
{
    public:
        Loader() : file(std::make_shared<std::ifstream>()) {};

        int checkFile(std::string &filename);

        QVector3D readQVector3D();
        QVector4D readQVector4D();
        QColor readQColor();

        Sphere readSphere();
        Bubble readSphereBubble();
        std::shared_ptr<TriangulatedObject> readTriangulatedObject();
        Light readLight();

        std::tuple<std::vector<std::shared_ptr<Object>>, std::vector<std::string>, std::vector<Light>> readFile();

    private:
        std::shared_ptr<std::ifstream> file = nullptr;
};

#endif // LOADER_H
