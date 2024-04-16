#ifndef _TESTS_H_
#define _TESTS_H_

#include <gtest/gtest.h>

#include <QVector3D>

#include "triangle.hpp"
#include "scene.hpp"

TEST(triangle_test, triangle_creating)
{
    QVector3D PointA(0, 0, 0);
    QVector3D PointB(1, 0, 0);
    QVector3D PointC(0, 1, 0);

    Triangle triangle(&PointA, &PointB, &PointC);
    EXPECT_EQ(*triangle.getA(), QVector3D(0, 0, 0));
    EXPECT_EQ(*triangle.getB(), QVector3D(1, 0, 0));
    EXPECT_EQ(*triangle.getC(), QVector3D(0, 1, 0));
}

TEST(triangle_test, normal_finding)
{
    QVector3D PointA(0, 0, 0);
    QVector3D PointB(1, 0, 0);
    QVector3D PointC(0, 1, 0);

    Triangle triangle(&PointA, &PointB, &PointC);
    triangle.findNormal(QVector3D(0, 0, 1));

    QVector3D normal = triangle.getNormal().normalized();
    
    EXPECT_EQ(normal, QVector3D(0, 0, -1));
}

TEST(SphereIntersectionTest, NoIntersection) {
    Sphere sphere(QVector3D(0, 0, 0), 1.0);
    QVector3D start_point(-2, -2, -2);
    QVector3D direction(-3, -3, -3);
    double min_dist = 0.0;
    double max_dist = 10.0;

    auto intersectionInfo = sphere.getIntersectionInfo(start_point, direction, min_dist, max_dist);

    EXPECT_EQ(std::get<0>(intersectionInfo), start_point);
    EXPECT_EQ(std::get<1>(intersectionInfo), start_point);
    EXPECT_EQ(std::get<2>(intersectionInfo), max_dist);
}

TEST(SphereIntersectionTest, SingleIntersection) {
    Sphere sphere(QVector3D(0, 0, 0), 1.0);
    QVector3D start_point(2, 0, 0);
    QVector3D direction(-1, 0, 0);
    double min_dist = 0.0;
    double max_dist = 10.0;

    auto intersectionInfo = sphere.getIntersectionInfo(start_point, direction, min_dist, max_dist);

    EXPECT_EQ(std::get<0>(intersectionInfo), QVector3D(1, 0, 0));
    EXPECT_EQ(std::get<2>(intersectionInfo), 1.0);
}

TEST(SphereIntersectionTest, TwoIntersections) {
    Sphere sphere(QVector3D(0, 0, 0), 2.0);
    QVector3D start_point(0, 3, 0);
    QVector3D direction(0, -1, 0);
    double min_dist = 0.0;
    double max_dist = 10.0;

    auto intersectionInfo = sphere.getIntersectionInfo(start_point, direction, min_dist, max_dist);

    EXPECT_EQ(std::get<0>(intersectionInfo), QVector3D(0, 2, 0));
    EXPECT_EQ(std::get<2>(intersectionInfo), 1.0);
}

TEST(scene_test, is_intersection)
{
    QVector3D PointA(0, 0, 0);
    QVector3D PointB(1, 0, 0);
    QVector3D PointC(0, 1, 0);

    Triangle triangle(&PointA, &PointB, &PointC);
    triangle.findNormal(QVector3D(0, 0, 1));

    QVector3D direction(0, 1, 0);

    bool res = triangle.isIntersection(direction);

    EXPECT_EQ(res, false);
}

#endif
