#include <sys/time.h>

#include "scene.hpp"
#include "my_math.hpp"

long long get_time_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}

std::tuple<int, int, std::vector<std::string>> Scene::loadScene(std::string filename)
{            
    if (sceneLoader.checkFile(filename))
        return {};

    auto [new_whole_objects, new_names, new_lights] = sceneLoader.readFile();

    for (auto &o : new_whole_objects)
        whole_objects.push_back(o);

    for (auto &l : new_lights)
        lights.push_back(l);

    return { (int)whole_objects.size(), (int)lights.size(), (std::vector<std::string>)new_names };
}

// Поиск вектора отражения
QVector3D Scene::getReflectVector(QVector3D viewVector, QVector3D normal)
{
    return (viewVector - normal * 2.0 * scalarProduct(viewVector, normal));
}

// Поиск вектора преломления
QVector3D Scene::getRefractVector(QVector3D viewVector, QVector3D normal, double r, double i, bool where)
{
    double cosI = -std::max<double>(-1., std::min<double>(1., scalarProduct(viewVector, normal)));
    if (cosI < 0)
        return getRefractVector(viewVector, normal * (-1), i, r, where);
    double c = i / r;
    double k = 1 - c * c * (1 - cosI * cosI);
    return k < 0 ? QVector3D(1, 0, 0) : viewVector * c + normal * (c * cosI - std::sqrt(k));
}

double getComponentColor(double intensity, double specIntensity, Coefficients coefficients, int color, int reflectColor, int refractColor, int lightsColor, double reflectK, double k)
{
    double c = intensity * color * coefficients.ambient + specIntensity * lightsColor * coefficients.diffuse + k * std::max(reflectColor * coefficients.specular * reflectK, refractColor * coefficients.refract);
   
    if (c > 255)
        c = 255;

    return c;
}

QColor Scene::getColor(double intensity, double specIntensity, Coefficients coefficients, QColor color, QColor reflectColor, QColor refractColor, double reflectK, int depth)
{
    QColor newColor;
    int color_r, color_g, color_b, reflect_r, reflect_g, reflect_b, refract_r, refract_g, refract_b, lights_r, lights_g, lights_b;

    color.getRgb(&color_r, &color_g, &color_b);
    reflectColor.getRgb(&reflect_r, &reflect_g, &reflect_b);
    refractColor.getRgb(&refract_r, &refract_g, &refract_b);
    lights_color.getRgb(&lights_r, &lights_g, &lights_b);

    double k = 1 / pow(1.05, depth);

    double red = getComponentColor(intensity, specIntensity, coefficients, color_r, reflect_r, refract_r, lights_r, reflectK, k);
    double green = getComponentColor(intensity, specIntensity, coefficients, color_g, reflect_g, refract_g, lights_g, reflectK, k);
    double blue = getComponentColor(intensity, specIntensity, coefficients, color_b, reflect_b, refract_b, lights_b, reflectK, k);

    newColor.setRed(red);
    newColor.setGreen(green);
    newColor.setBlue(blue);

    return newColor;
}

std::tuple<int, int, QVector3D, QVector3D> Scene::findClosestIntersection(QVector3D start_point, QVector3D direction, double min_dist, double max_dist)
{
    double min_t = max_dist;

    int closest_num = 0;
    QVector3D closest_point(0, 0, 0);
    QVector3D closest_normal(0, 0, 0);
    int flag = NOTHING;

    for (int i = 0; i < objects.size(); i++)
    {
        auto [point, normal, t] = objects[i]->getIntersectionInfo(start_point, direction, min_dist, max_dist);

        if (t < min_t && t > min_dist && t < max_dist)
        {
            min_t = t;
            closest_num = i;
            closest_point = point;
            closest_normal = normal;
            flag = SOMETHING;
        }
    }

    return { flag, closest_num, closest_point, closest_normal };
}

double getInterferenceColor(QColor c, Substance s)
{
    double rC;

    if (c.red() > 0)
        rC = s.getRefractiveIndices()[0];
    if (c.green() > 0)
        rC = s.getRefractiveIndices()[1];
    else
        rC = s.getRefractiveIndices()[2];

    return rC;
}

double getInterferenceStrengthCoef(double t, double rC, QVector3D dir, QVector3D n, double l)
{
    double d = -2 * t * rC * std::max<double>(-1., std::min<double>(1., scalarProduct(dir, n))) + l / 2;
    double del = 2 * M_PI * d / l;
    del /= 1000000000;

    double interferenceStrength = 2 * pow(cos(del / 2), 2);

    return interferenceStrength;
}

double Scene::findLightIntensity(QVector3D start_point, QVector3D direction, double min_dist, double max_dist)
{
    double intensity = 1;
    double rc;

    for (int i = 0; i < objects.size(); i++)
    {
        auto [point, normal, t] = objects[i]->getIntersectionInfo(start_point, direction, min_dist, max_dist);

        if (t > min_dist && t < max_dist)
        {
            rc = objects[i]->getCoefficients().refract;
            
            if (!rc)
                return 0;

            intensity *= rc;
        }
    }

    return intensity;
}

QColor Scene::TraceRay(Ray ray, double min_dist, double max_dist, int trace_depth, bool where)
{
    QColor color = QColor(0, 0, 0);

    auto [flag, closest, intersection, normal] = findClosestIntersection(ray.start_point, ray.direction, min_dist, max_dist);

    if (flag == NOTHING)
        return color;

    // if (where == IN)
    //     normal = -normal;

    // По идее это надо переместить пониже и проверить на то, что нет преломления и отражения
    if (trace_depth == max_trace_depth)
        return color;

    double reflectK = 0.0;
    QColor reflectColor = QColor(0, 0, 0);

    // Отражение
    if (objects[closest]->getSubstance().getCoefficients().specular > EPS)
    {
        QVector3D reflectDir = getReflectVector(ray.direction, normal).normalized();

        Ray reflectRay = {
            .start_point = intersection,
            .direction = reflectDir,
            .color = ray.color,
            .len = ray.len,
            .n = ray.n,
        };

        reflectColor = TraceRay(reflectRay, min_dist, max_dist, trace_depth + 1, where);

        double rC = getInterferenceColor(ray.color, objects[closest]->getSubstance());

        reflectK = getInterferenceStrengthCoef(objects[closest]->getThickness(), rC, ray.direction, normal, ray.len);
    }

    QColor refractColor = QColor(0, 0, 0);

    // Преломление
    if (objects[closest]->getSubstance().getCoefficients().refract > EPS) 
    {
        double rC = getInterferenceColor(ray.color, objects[closest]->getSubstance());

        QVector3D refractDir = getRefractVector(ray.direction, normal, rC, ray.n, where).normalized();

        Ray refractRay = {
            .start_point = intersection,
            .direction = refractDir,
            .color = ray.color,
            .len = ray.len,
            .n = rC,
        };

        refractColor = TraceRay(refractRay, min_dist, max_dist, trace_depth + 1, !where);
    }

    double intensity = 0.0;
    double specIntensity = 0.0;

    // Тени + модель освещения
    for (int i = 0; i < lights.size(); i++) 
    {
        QVector3D light = intersection - lights[i].getView();
        
        double k = findLightIntensity(intersection, (-1) * light, MIN_DIST_LIGHT, MAX_DIST_LIGHT);

        if (!k)
            continue;

        light.normalize();
        
        intensity += k * lights[i].getIntensity() * std::max(0., scalarProduct(normal, (-1) * light));
        QVector3D reflect = (getReflectVector(light, normal)).normalized();
        specIntensity += pow(std::max(0., scalarProduct(reflect, (-1) * ray.direction)), objects[closest]->getSubstance().getSpecularIndex()) * lights[i].getIntensity() * k;
    }

    QColor diffColor = objects[closest]->getSubstance().getColor();

    color = getColor(intensity, specIntensity, objects[closest]->getSubstance().getCoefficients(), diffColor, reflectColor, refractColor, reflectK, trace_depth);

    return color;
}

void Scene::drawThread(int x_start, int x_end, int y_start, int y_end, int trace_depth)
{
     for (int x = x_start; x < x_end; x++)
        for (int y = y_start; y < y_end; y++)
        {
            Ray ray;

            int red = 0;
            int green = 0;
            int blue = 0;

            int del = std::sqrt(RAYS);
            int correct = del / 2;

            for (int i = 0; i < RAYS; i++)
            {
                float new_x = x + (i % del - correct) / 2.;
                float new_y = y + (i / del - correct) / 2.;
                QVector3D point(new_x, new_y, SCREEN_Z);

                QVector3D direction = (point - eyePosition).normalized();

                ray.start_point = point;
                ray.direction = direction;

                ray.color = QColor(255, 0, 0);
                ray.len = RC;
                red += TraceRay(ray, MIN_DIST, MAX_DIST, trace_depth, OUT).red();

                ray.color = QColor(0, 255, 0);
                ray.len = GC;
                green += TraceRay(ray, MIN_DIST, MAX_DIST, trace_depth, OUT).green();

                ray.color = QColor(0, 0, 255);
                ray.len = BC;
                blue += TraceRay(ray, MIN_DIST, MAX_DIST, trace_depth, OUT).blue();
            }

            // double new_red = std::sqrt(red / RAYS / 255.);
            // double new_green = std::sqrt(green / RAYS / 255.);
            // double new_blue = std::sqrt(blue / RAYS / 255.);

            // image->setPixel(x, y, qRgb(int((new_red * 255)), int((new_green * 255)), int((new_blue * 255))));

            image->setPixel(x, y, qRgb(red / RAYS, green / RAYS, blue / RAYS));
        }
}

void Scene::draw(int trace_depth)
{
    long long start_time = get_time_ms();

    if (trace_depth == -1)
        max_trace_depth = TRACE_DEPTH;
    else
        max_trace_depth = trace_depth;

    image->fill(QColor(0, 0, 0));

    for (auto object: whole_objects)
    {
        std::vector<std::shared_ptr<Object>> split_object = object->split();
        for (auto o : split_object)
            objects.push_back(o);
    }

    std::vector<std::thread> threads(THREADS_COUNT);

    int count_y = canvasHeight / THREADS_COUNT;
    int dop_count_y = canvasHeight % THREADS_COUNT - 2;

    int start_y = 0;
    int end_y = count_y + (dop_count_y > 0);

    for (int i = 0; i < THREADS_COUNT; i++)
    {
        threads[i] = std::thread(&Scene::drawThread, this, 0, canvasWidth, start_y, end_y, 0);

        start_y = end_y;
        end_y = start_y + count_y + (i < dop_count_y);
    }

    for (int i = 0; i < THREADS_COUNT; i++)
        threads[i].join();

    long long end_time = get_time_ms();

    objects.clear();

    // std::cout << "Трассировка выполнилась за " << end_time - start_time << " мс при количестве потоков " << THREADS_COUNT << "\n";
}
