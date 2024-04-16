#include "loader.hpp"

int Loader::checkFile(std::string &filename)
{
    file->open(filename);

    if (file->fail())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

QVector3D Loader::readQVector3D()
{
    std::string num[3];

    for (int i = 0; i < 3; i++)
        *file >> num[i];

    return QVector3D(stod(num[0]), stod(num[1]), stod(num[2]));
}

QVector4D Loader::readQVector4D()
{
    std::string num[4];

    for (int i = 0; i < 4; i++)
        *file >> num[i];

    return QVector4D(stod(num[0]), stod(num[1]), stod(num[2]), stod(num[3]));
}

QColor Loader::readQColor()
{
    std::string num[3];

    for (int i = 0; i < 3; i++)
        *file >> num[i];

    return QColor(stoi(num[0]), stoi(num[1]), stoi(num[2]));
}

Sphere Loader::readSphere()
{
    std::string radius, specularIndex, refractiveIndex, dispersion;

    QVector3D center = readQVector3D();
    *file >> radius;
    QColor color = readQColor();
    QVector4D tmp_coefficients = readQVector4D();
    Coefficients coefficients = { tmp_coefficients.x(),  tmp_coefficients.y(), tmp_coefficients.z(), tmp_coefficients.w() };
    *file >> specularIndex;
    *file >> refractiveIndex;
    *file >> dispersion;

    Sphere s = Sphere(center, stod(radius));
    s.setSubstance(color, coefficients, stod(specularIndex), stod(refractiveIndex), stod(dispersion));

    return s;
}

Bubble Loader::readSphereBubble()
{
    Bubble b;

    Sphere s1 = readSphere();
    Sphere s2 = readSphere();
    double thickness = s1.getRadius() - s2.getRadius();
    s1.setThickness(thickness);
    s2.setThickness(thickness);
    b.setThickness(thickness);

    b.setOuter(std::make_shared<Sphere>(s1));
    b.setInner(std::make_shared<Sphere>(s2));

    return b;
}

std::shared_ptr<TriangulatedObject> Loader::readTriangulatedObject()
{
    std::string num[3];
    std::string string;
    std::vector<QVector3D> points;
    std::vector<Triangle> faces;
    std::string specularIndex, refractiveIndex, dispersion;

    std::shared_ptr<TriangulatedObject> triangulatedObject = std::make_shared<TriangulatedObject>();

    *file >> string;

    QVector3D points_sum(0, 0, 0);

    while (string == "v")
    {
        for (int i = 0; i < 3; i++)
            *file >> num[i];

        QVector3D point(stod(num[0]), stod(num[1]), stod(num[2]));
        points.push_back(point);

        points_sum += point;

        *file >> string;
    }

    double coef = 1. / points.size();
    QVector3D barycenter(coef * points_sum);
    triangulatedObject->setCenter(barycenter);

    triangulatedObject->setPoints(points);

    while (string == "f")
    {
        for (int i = 0; i < 3; i++)
            *file >> num[i];

        Triangle face(triangulatedObject->getPointAt(stoi(num[0]) - 1),
                      triangulatedObject->getPointAt(stoi(num[1]) - 1),
                      triangulatedObject->getPointAt(stoi(num[2]) - 1));

        faces.push_back(face);

        *file >> string;
    }

    for (int i = 0; i < faces.size(); i++)
        faces[i].findNormal(barycenter);

    triangulatedObject->setFaces(faces);

    QColor color = readQColor();
    QVector4D tmp_coefficients = readQVector4D();
    Coefficients coefficients = { tmp_coefficients.x(),  tmp_coefficients.y(), tmp_coefficients.z(), tmp_coefficients.w() };
    *file >> specularIndex;
    *file >> refractiveIndex;
    *file >> dispersion;

    triangulatedObject->setSubstance(color, coefficients, stod(specularIndex), stod(refractiveIndex), stod(dispersion));

    return triangulatedObject;
}

Light Loader::readLight()
{
    double intensity;

    QVector3D view = readQVector3D();
    *file >> intensity;

    Light l = Light(view, intensity);
    return l;
}

std::tuple<std::vector<std::shared_ptr<Object>>, std::vector<std::string>, std::vector<Light>> Loader::readFile()
{
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<Light> lights;

    std::string string;
    std::vector<std::string> names;

    while(!file->eof())
    {
        if (string == "t")
        {
            *file >> string;
            names.push_back(string);

            std::shared_ptr<TriangulatedObject> t = readTriangulatedObject();

            objects.push_back(t);

            *file >> string;
        }
        else if (string == "s")
        {
            *file >> string;
            names.push_back(string);

            Sphere s = readSphere();
            objects.push_back(std::make_shared<Sphere>(s));
            *file >> string;
        }
        else if (string == "b")
        {
            *file >> string;
            names.push_back(string);

            Bubble b = readSphereBubble();
            objects.push_back(std::make_shared<Bubble>(b));
            *file >> string;
        }
        else if (string == "l")
        {
            QVector3D view = readQVector3D();
            std::string intensity;
            *file >> intensity;
            lights.push_back(Light(view, stod(intensity)));
            *file >> string;
        }
        else
        {
            *file >> string;
        }
    }

    return {objects, names, lights};
}
