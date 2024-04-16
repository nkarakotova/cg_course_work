#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <string>
#include <time.h>

#include <QMainWindow>
#include <QApplication>
#include <QFileDialog>
#include <QPixmap>
#include <QColorDialog>
#include <QString>
#include <QTableWidgetItem>

#include "loader.hpp"
#include "scene.hpp"
#include "ui_mainwindow.h"

#define sceneDefaultWidth 960
#define sceneDefaultHeight 960

#define EXIT_PROGRAM 1

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent), ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        scene = Scene(sceneDefaultWidth, sceneDefaultHeight);
        scene_is_present = true;
    };

    ~MainWindow() { delete ui; };

    void setScene(std::string const &filename);
    int getImage(std::string const &filename, int trace_depth = -1);

    void setInputFilename(std::string const &filename) { inputFilename = filename; }
    bool isInputFilename() { return !inputFilename.empty(); };

    void setOutputFilename(std::string const &filename) { outputFilename = filename; }

    void moveObject(int index, double x, double y, double z) { scene.moveObject(index, x, y, z); };
    void scaleObject(int index, double n) { scene.scaleObject(index, n); };
    void rotateObject(int index, double x, double y, double z) { scene.rotateObject(index, x, y, z); };

    void changeObjectColor(int index, QColor newColor) { scene.changeObjectColor(index, newColor); };

    void changeCoefficients(int index, Coefficients newCoefficients) { scene.changeCoefficients(index, newCoefficients); };
    void changeSpecularIndex(int index, double newSpecularIndex) { scene.changeSpecularIndex(index, newSpecularIndex); };
    void changeRefractiveIndex(int index, double newRefractiveIndex) { scene.changeRefractiveIndex(index, newRefractiveIndex); };
    void changeDispersion(int index, double newDispersion) { scene.changeDispersion(index, newDispersion); };

    QColor getObjectColor(int index) { return scene.getObjectColor(index); };

    Coefficients getCoefficients(int index) { return scene.getCoefficients(index); };
    double getSpecularIndex(int index) { return scene.getSpecularIndex(index); };
    double getRefractiveIndex(int index) { return scene.getRefractiveIndex(index); };
    double getDispersion(int index) { return scene.getDispersion(index); };

    void setLightPosition(int index, QVector3D newPosition) { scene.setLightPosition(index, newPosition); };
    void setIntensity(int index, double newIntensity) { scene.setIntensity(index, newIntensity); };

    QVector3D getLightPosition(int index) { return scene.getLightPosition(index); };
    double getIntensity(int index) { return scene.getIntensity(index); };

    void delLightByIndex(int index) { scene.delLightByIndex(index); };
    void addLight(QVector3D newPosition, double newIntensity) { scene.addLight(newPosition, newIntensity); };

private slots:
    void on_load_clicked();
    void on_draw_clicked();
    void on_clean_clicked();
    void on_change_object_color_clicked();
    void on_objects_list_currentRowChanged(int currentRow);
    void on_change_parametrs_clicked();
    void on_change_geometry_clicked();
    void on_lights_list_currentRowChanged(int currentRow);
    void on_change_light_clicked();
    void on_del_light_clicked();
    void on_add_light_clicked();

private:
    Ui::MainWindow *ui;
    std::string inputFilename;
    std::string outputFilename;
    Scene scene;

    bool scene_is_present = false;

    int objects_count = 0, lights_count = 0;
};
#endif // MAINWINDOW_H
