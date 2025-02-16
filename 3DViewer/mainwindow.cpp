#include "mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <iostream>

#include "ui_mainwindow.h"
#include "widget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      recording(false),
      settings("MyCompany", "MyApp") {
  ui->setupUi(this);
  gifTimer = new QTimer();
  connect(gifTimer, SIGNAL(timeout()), this, SLOT(gif_timer()));

  create_obj(&model);

  loadSettings();
}

MainWindow::~MainWindow() {
  saveSettings();
  clean_obj(&model);

  delete ui;
}
void MainWindow::saveSettings() {
  settings.setValue("modelColor", modelColor);
  settings.setValue("edgeColor", edgeColor);
  settings.setValue("vertexColor", vertexColor);
  settings.setValue("edgeWidth", edgeWidth);
  settings.setValue("vertexSize", vertexSize);
  settings.setValue("projectionType", projectionType);
  settings.setValue("edgeType", edgeType);
  settings.setValue("vertexShape", vertexShape);
  settings.setValue("backgroundColor", backgroundColor);
}

void MainWindow::loadSettings() {
  modelColor = settings.value("modelColor", QColor(Qt::white)).value<QColor>();
  edgeColor = settings.value("edgeColor", QColor(Qt::black)).value<QColor>();
  vertexColor =
      settings.value("vertexColor", QColor(Qt::black)).value<QColor>();
  edgeWidth = settings.value("edgeWidth", 1).toInt();
  vertexSize = settings.value("vertexSize", 1).toInt();
  projectionType = settings.value("projectionType", "Parallel").toString();
  edgeType = settings.value("edgeType", "Solid").toString();
  vertexShape = settings.value("vertexShape", "None").toString();
  backgroundColor =
      settings.value("backgroundColor", QColor(Qt::white)).value<QColor>();
  initPanel();
}
void MainWindow::initPanel() {
  ui->LineWidthSlider->setValue(edgeWidth);
  ui->PointSizeSlider->setValue(vertexSize);
  ui->openGLWidget->setModelColor(modelColor);
  ui->openGLWidget->setBackgroundColor(backgroundColor);
  ui->openGLWidget->setEdgeColor(edgeColor);
  ui->openGLWidget->setVertexColor(vertexColor);
  ui->openGLWidget->setVertexSize(vertexSize);
  ui->openGLWidget->setEdgeWidth(edgeWidth);
  ui->openGLWidget->setProjectionType(projectionType);
  ui->openGLWidget->setEdgeType(edgeType);
  ui->openGLWidget->setVertexShape(vertexShape);
  if (projectionType == "Parallel") {
    ui->ParralelRadioButton->setChecked(true);
  } else {
    ui->perspectiveRadioButton->setChecked(true);
  }
  if (edgeType == "Solid") {
    ui->SolidRadioButton->setChecked(true);
  } else {
    ui->dottedRadioButton->setChecked(true);
  }
  if (vertexShape == "None") {
    ui->EmptyRadioButton->setChecked(true);
  } else if (vertexShape == "Circle") {
    ui->CircleRadioButton->setChecked(true);
  } else if (vertexShape == "Square") {
    ui->SquareRadioButton->setChecked(true);
  }
}
void MainWindow::on_FileButton_clicked() {
  std::cout << "File button clicked.";
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open OBJ File"), "",
                                                  tr("OBJ Files (*.obj)"));
  if (!fileName.isEmpty()) {
    qDebug() << "Selected file:" << fileName;

    ui->openGLWidget->makeCurrent();
    ui->openGLWidget->draw_obj(fileName.toStdString().c_str());
    ui->openGLWidget->update();
    ui->PathLabel->setText("Path:" + fileName);
    model = ui->openGLWidget->getModel();
    QString vertexStr = "vertex: " + QString::number(model.num_vertices);
    ui->VertexLabel->setText(vertexStr);

    QString faceStr = "face: " + QString::number(model.num_faces);
    ui->FacetesLabel->setText(faceStr);
  } else {
    qDebug() << "No file selected.";
  }
}

void MainWindow::transformAndDraw() {
  ui->openGLWidget->makeCurrent();
  ui->openGLWidget->draw(model);
  ui->openGLWidget->update();
}
void MainWindow::on_xTransitionSlider_valueChanged(int value) {
  if (model.num_vertices != 0 && currentValues.xTranslation != value) {
    currentValues.xTranslation = value;
    ui->xTransitionSpinBox->setValue(value);
    move_x(&model, value);
    transformAndDraw();
  }
}

void MainWindow::on_xTransitionSpinBox_valueChanged(double arg1) {
  if (model.num_vertices != 0 && currentValues.xTranslation != arg1) {
    currentValues.xTranslation = arg1;
    ui->xTransitionSlider->setValue(arg1);
    move_x(&model, arg1);
    transformAndDraw();
  }
}

void MainWindow::on_yTransitionSlider_valueChanged(int value) {
  if (model.num_vertices != 0 && currentValues.yTranslation != value) {
    currentValues.yTranslation = value;
    ui->yTransitionSpinBox->setValue(value);
    move_y(&model, value);
    transformAndDraw();
  }
}

void MainWindow::on_yTransitionSpinBox_valueChanged(double arg1) {
  if (model.num_vertices != 0 && currentValues.yTranslation != arg1) {
    currentValues.yTranslation = arg1;
    ui->yTransitionSlider->setValue(arg1);
    move_y(&model, arg1);
    transformAndDraw();
  }
}

void MainWindow::on_zTransitionSlider_valueChanged(int value) {
  if (model.num_vertices != 0 && currentValues.zTranslation != value) {
    currentValues.zTranslation = value;
    ui->zTransitionSpinBox->setValue(value);
    move_z(&model, value);
    transformAndDraw();
  }
}

void MainWindow::on_zTransitionSpinBox_valueChanged(double arg1) {
  if (model.num_vertices != 0 && currentValues.zTranslation != arg1) {
    currentValues.zTranslation = arg1;
    ui->zTransitionSlider->setValue(arg1);
    move_z(&model, arg1);
    transformAndDraw();
  }
}

void MainWindow::on_xRotationSlider_valueChanged(int value) {
  if (model.num_vertices != 0 && currentValues.xRotation != value) {
    currentValues.xRotation = value;
    ui->xRotationSpinBox->setValue(value);
    rotate_x(&model, value);
    transformAndDraw();
  }
}

void MainWindow::on_xRotationSpinBox_valueChanged(double arg1) {
  if (model.num_vertices != 0 && currentValues.xRotation != arg1) {
    currentValues.xRotation = arg1;
    ui->xRotationSlider->setValue(arg1);
    rotate_x(&model, arg1);
    transformAndDraw();
  }
}

void MainWindow::on_yRotationSlider_valueChanged(int value) {
  if (model.num_vertices != 0 && currentValues.yRotation != value) {
    currentValues.yRotation = value;
    ui->yRotationSpinBox->setValue(value);
    rotate_y(&model, (double)value);
    transformAndDraw();
  }
}

void MainWindow::on_yRotationSpinBox_valueChanged(double arg1) {
  if (model.num_vertices != 0 && currentValues.yRotation != arg1) {
    currentValues.yRotation = arg1;
    ui->yRotationSlider->setValue(arg1);
    rotate_y(&model, arg1);
    transformAndDraw();
  }
}

void MainWindow::on_zRotationSlider_valueChanged(int value) {
  if (model.num_vertices != 0 && currentValues.zRotation != value) {
    currentValues.zRotation = value;
    ui->zRotationSpinBox->setValue(value);
    rotate_z(&model, value);
    transformAndDraw();
  }
}

void MainWindow::on_zRotationSpinBox_valueChanged(double arg1) {
  if (model.num_vertices != 0 && currentValues.zRotation != arg1) {
    currentValues.zRotation = arg1;
    ui->zRotationSlider->setValue(arg1);
    rotate_z(&model, arg1);
    transformAndDraw();
  }
}

void MainWindow::on_ScaleSlider_valueChanged(int value) {
  double scaleValue = value;
  if (model.num_vertices != 0 && currentValues.scale != scaleValue) {
    currentValues.scale = scaleValue;
    ui->ScaleSpinBox->setValue(scaleValue);
    scale(&model, scaleValue);
    transformAndDraw();
  }
}

void MainWindow::on_ScaleSpinBox_valueChanged(double arg1) {
  if (model.num_vertices != 0 && currentValues.scale != arg1 &&
      std::abs(arg1) > std::numeric_limits<double>::epsilon()) {
    currentValues.scale = arg1;
    ui->ScaleSlider->setValue(arg1);
    scale(&model, arg1);
    transformAndDraw();
  }
}

void MainWindow::on_ScrenshotButton_clicked() {
  QImage screenshot = ui->openGLWidget->grabFramebuffer();

  QString filter = "BMP files (*.bmp);;JPEG files (*.jpg)";
  QString fileName =
      QFileDialog::getSaveFileName(this, tr("Save Screenshot"), "", filter);

  if (!fileName.isEmpty()) {
    QFileInfo fileInfo(fileName);
    QString extension = fileInfo.suffix().toLower();

    if (extension == "bmp") {
      screenshot.save(fileName, "BMP");
    } else if (extension == "jpg" || extension == "jpeg") {
      screenshot.save(fileName, "JPEG");
    } else {
      screenshot.save(fileName + ".jpg", "JPEG");
    }
  }
}

void MainWindow::on_GifButton_clicked() {
  if (!recording) {
    startRecording();
  } else {
    stopRecording();
  }
}
void MainWindow::startRecording() {
  // Меняем текст кнопки на "Save"
  ui->GifButton->setText("Save");
  recording = true;

  gifOut = new QGifImage();
  gifTimer->start(100);
}

void MainWindow::stopRecording() {
  gifTimer->stop();
  recording = false;
  gifTime = 0;
  QString safeGIF = QFileDialog::getSaveFileName(this, "Сохранить как...", "",
                                                 "GIF Files (*.gif)");
  if (!safeGIF.isNull()) gifOut->save(safeGIF);
  gifOut->~QGifImage();
  ui->GifButton->setText("Start GIF Recording");
}

void MainWindow::gif_timer() {
  qDebug() << gifTime++;

  QImage frame = ui->openGLWidget->grabFramebuffer();
  frame = frame.scaled(640, 480, Qt::IgnoreAspectRatio);
  gifOut->addFrame(frame, 100);
}

void MainWindow::on_pushButton_clicked() {
  QColor color =
      QColorDialog::getColor(backgroundColor, this, "Choose background color");
  if (color.isValid()) {
    backgroundColor = color;
    ui->openGLWidget->setBackgroundColor(backgroundColor);
    transformAndDraw();
  }
}

void MainWindow::on_ModelColorButton_clicked() {
  QColor color = QColorDialog::getColor(modelColor, this, "Choose model color");
  if (color.isValid()) {
    modelColor = color;
    // setColor(false, modelColor.redF(), modelColor.greenF(),
    // modelColor.blueF());
    ui->openGLWidget->setModelColor(modelColor);
    transformAndDraw();
  }
}

void MainWindow::on_perspectiveRadioButton_clicked(bool checked) {
  if (model.num_vertices != 0) {
    if (checked) {
      projectionType = "Perspective";
      ui->openGLWidget->setProjectionType(projectionType);
      transformAndDraw();
    }
  }
}

void MainWindow::on_VertexColorButton_clicked() {
  QColor color =
      QColorDialog::getColor(vertexColor, this, "Choose vertex color");
  if (color.isValid()) {
    vertexColor = color;
    ui->openGLWidget->setVertexColor(vertexColor);
    transformAndDraw();
  }
}

void MainWindow::on_FadeColorButton_clicked() {
  QColor color = QColorDialog::getColor(edgeColor, this, "Choose face color");
  if (color.isValid()) {
    edgeColor = color;
    ui->openGLWidget->setEdgeColor(edgeColor);
    transformAndDraw();
  }
}

void MainWindow::on_dottedRadioButton_clicked(bool checked) {
  if (model.num_vertices != 0) {
    if (checked) {
      edgeType = "Dashed";
      ui->openGLWidget->setEdgeType(edgeType);
      transformAndDraw();
    }
  }
}

void MainWindow::on_LineWidthSlider_valueChanged(int value) {
  if (model.num_vertices != 0) {
    edgeWidth = value;
    ui->openGLWidget->setEdgeWidth(edgeWidth);
    transformAndDraw();
  }
}

void MainWindow::on_PointSizeSlider_valueChanged(int value) {
  if (model.num_vertices != 0) {
    vertexSize = value;
    ui->openGLWidget->setVertexSize(vertexSize);
    transformAndDraw();
  }
}

void MainWindow::on_EmptyRadioButton_toggled(bool checked) {
  if (model.num_vertices != 0) {
    if (checked) {
      vertexShape = "None";
      ui->openGLWidget->setVertexShape(vertexShape);
      transformAndDraw();
    }
  }
}

void MainWindow::on_CircleRadioButton_toggled(bool checked) {
  if (model.num_vertices != 0) {
    if (checked) {
      vertexShape = "Circle";
      ui->openGLWidget->setVertexShape(vertexShape);
      transformAndDraw();
    }
  }
}

void MainWindow::on_SquareRadioButton_toggled(bool checked) {
  if (model.num_vertices != 0) {
    if (checked) {
      vertexShape = "Square";
      ui->openGLWidget->setVertexShape(vertexShape);
      transformAndDraw();
    }
  }
}

void MainWindow::on_SolidRadioButton_toggled(bool checked) {
  if (model.num_vertices != 0) {
    if (checked) {
      edgeType = "Solid";
      ui->openGLWidget->setEdgeType(edgeType);
      transformAndDraw();
    }
  }
}

void MainWindow::on_ParralelRadioButton_clicked(bool checked) {
  if (model.num_vertices != 0) {
    if (checked) {
      projectionType = "Parallel";
      ui->openGLWidget->setProjectionType(projectionType);
      transformAndDraw();
    }
  }
}
