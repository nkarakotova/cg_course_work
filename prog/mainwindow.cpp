#include "mainwindow.hpp"

void MainWindow::on_load_clicked()
{
    if (!scene_is_present) {
        scene = Scene(sceneDefaultWidth, sceneDefaultHeight);
        scene_is_present = true;
    }

    auto filename = QFileDialog::getOpenFileName();
    setInputFilename(filename.toStdString());

    Loader load;
    scene.setLoader(load);

    auto [new_objects_count, new_lights_count, new_names] = scene.loadScene(inputFilename);

    for (int i = objects_count, j = 0; i < new_objects_count; ++i, ++j)
    {
        ui->objects_list->addItem(QString::fromStdString(std::to_string(i + 1) + ") " + new_names[j]));
    }

    for (int i = lights_count; i < new_lights_count; ++i)
    {
        ui->lights_list->addItem(QString::number(i + 1));
    }

    objects_count += new_objects_count;
    lights_count += new_lights_count;

    scene.initializedImage();
}

void MainWindow::setScene(std::string const &filename)
{
    if (!scene_is_present) {
        scene = Scene(sceneDefaultWidth, sceneDefaultHeight);
        scene_is_present = true;
    }

    setInputFilename(filename);

    Loader load;
    scene.setLoader(load);
    scene.loadScene(inputFilename);
    scene.addLight(Light(QVector3D(100, 250, 3000), 1));
    scene.initializedImage();
}

void MainWindow::on_draw_clicked()
{
    if (!isInputFilename() || !scene_is_present)
        return;

    scene.draw();
    std::shared_ptr<QImage> currentImage = (scene.getImage());

    QGraphicsScene* scene = new QGraphicsScene(this);
    ui->canvas->setScene(scene);

    QPixmap pixmap = QPixmap::fromImage(*currentImage);

    ui->canvas->scene()->addPixmap(pixmap);

    std::cout << "Done!\n";
}

int MainWindow::getImage(std::string const &filename, int trace_depth)
{
    int start, end;

    setOutputFilename(filename);

    start = clock();
    scene.draw(trace_depth);
    end = clock();

    std::shared_ptr<QImage> currentImage = (scene.getImage());

    QGraphicsScene* scene = new QGraphicsScene(this);
    ui->canvas->setScene(scene);

//    QPixmap pixmap = QPixmap::fromImage(*currentImage);

    currentImage->save(outputFilename.c_str(), "PNG");

    return end - start;
}

void MainWindow::on_change_object_color_clicked()
{
    QColor new_color = QColorDialog::getColor();

    QString s("background: #"
              + QString(new_color.red() < 16? "0" : "") + QString::number(new_color.red(),16)
              + QString(new_color.green() < 16? "0" : "") + QString::number(new_color.green(),16)
              + QString(new_color.blue() < 16? "0" : "") + QString::number(new_color.blue(),16) + ";");

    ui->change_object_color->setStyleSheet(s);
    ui->change_object_color->update();

    changeObjectColor(ui->objects_list->currentRow(), new_color);
    on_draw_clicked();
}

void MainWindow::on_clean_clicked()
{
    ui->canvas->scene()->clear();

    if (ui->objects_list) {
        ui->objects_list->blockSignals(true);
        ui->objects_list->clear();
        ui->objects_list->blockSignals(false);
    }

    if (ui->lights_list) {
        ui->lights_list->blockSignals(true);
        ui->lights_list->clear();
        ui->lights_list->blockSignals(false);
    }

    scene_is_present = false;
    objects_count = 0;
    lights_count = 0;
}

void MainWindow::on_objects_list_currentRowChanged(int currentRow)
{
    QColor new_color = getObjectColor(currentRow);

    QString s("background: #"
              + QString(new_color.red() < 16? "0" : "") + QString::number(new_color.red(),16)
              + QString(new_color.green() < 16? "0" : "") + QString::number(new_color.green(),16)
              + QString(new_color.blue() < 16? "0" : "") + QString::number(new_color.blue(),16) + ";");

    ui->change_object_color->setStyleSheet(s);

    Coefficients coefficients = getCoefficients(currentRow);

    ui->change_ambient_coefficient->setValue(coefficients.ambient);
    ui->change_diffuse_coefficient->setValue(coefficients.diffuse);
    ui->change_specular_coefficient->setValue(coefficients.specular);
    ui->change_refract_coefficient->setValue(coefficients.refract);

    ui->change_specular_index->setValue(getSpecularIndex(currentRow));
    ui->change_refract_index->setValue(getRefractiveIndex(currentRow));
    ui->change_dispersion->setValue(getDispersion(currentRow));
}


void MainWindow::on_change_parametrs_clicked()
{
    int currentRow = ui->objects_list->currentRow();

    Coefficients coefficients = { ui->change_ambient_coefficient->value(),
                                  ui->change_diffuse_coefficient->value(),
                                  ui->change_specular_coefficient->value(),
                                  ui->change_refract_coefficient->value() };

    changeCoefficients(currentRow, coefficients);
    changeSpecularIndex(currentRow, ui->change_specular_index->value());
    changeRefractiveIndex(currentRow, ui->change_refract_index->value());
    changeDispersion(currentRow, ui->change_dispersion->value());

    on_draw_clicked();
}


void MainWindow::on_change_geometry_clicked()
{
    if (ui->objects_list->selectedItems().empty()) {
        return;
    }
    int currentRow = ui->objects_list->currentRow();

    if (ui->move_x->value() != 0 || ui->move_y->value() != 0 || ui->move_z->value() != 0)
        moveObject(currentRow, ui->move_x->value(), ui->move_y->value(), ui->move_z->value());

    if (ui->scale_n->value() != 0)
        scaleObject(currentRow, ui->scale_n->value());

    if (ui->rotate_x->value() != 0 || ui->rotate_y->value() != 0 || ui->rotate_z->value() != 0)
        rotateObject(currentRow, ui->rotate_x->value(), ui->rotate_y->value(), ui->rotate_z->value());

    on_draw_clicked();
}


void MainWindow::on_lights_list_currentRowChanged(int currentRow)
{
    QVector3D coordinates = getLightPosition(currentRow);

    ui->light_x->setValue(coordinates.x());
    ui->light_y->setValue(coordinates.y());
    ui->light_z->setValue(coordinates.z());

    ui->light_intensity->setValue(getIntensity(currentRow));
}


void MainWindow::on_change_light_clicked()
{
    int currentRow = ui->lights_list->currentRow();
    QVector3D coordinates(ui->light_x->value(), ui->light_y->value(), ui->light_z->value());
    double intensity = ui->light_intensity->value();

    setLightPosition(currentRow, coordinates);
    setIntensity(currentRow, intensity);

    on_draw_clicked();
}


void MainWindow::on_del_light_clicked()
{
    int currentRow = ui->lights_list->currentRow();

    delLightByIndex(currentRow);
    ui->lights_list->takeItem(currentRow);
    --lights_count;

    on_draw_clicked();
}


void MainWindow::on_add_light_clicked()
{
    QVector3D coordinates(ui->light_x->value(), ui->light_y->value(), ui->light_z->value());
    double intensity = ui->light_intensity->value();

    addLight(coordinates, intensity);

    ui->lights_list->addItem(QString::number(lights_count));
    ++lights_count;

    on_draw_clicked();
}
