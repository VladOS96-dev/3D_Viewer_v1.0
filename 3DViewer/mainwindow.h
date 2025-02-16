#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <stdlib.h>

#include <QColorDialog>
#include <QMainWindow>
#include <QSettings>
#include <QTimer>
#include <cmath>
extern "C" {
#include "obj_parcer.h"
}
#include <qgifimage.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  bool isRecording() const;
 private slots:
  void gif_timer();

  void on_FileButton_clicked();

  void on_xTransitionSlider_valueChanged(int value);

  void on_xTransitionSpinBox_valueChanged(double arg1);

  void on_yTransitionSlider_valueChanged(int value);

  void on_yTransitionSpinBox_valueChanged(double arg1);

  void on_zTransitionSlider_valueChanged(int value);

  void on_zTransitionSpinBox_valueChanged(double arg1);

  void on_xRotationSlider_valueChanged(int value);

  void on_xRotationSpinBox_valueChanged(double arg1);

  void on_yRotationSlider_valueChanged(int value);

  void on_yRotationSpinBox_valueChanged(double arg1);

  void on_zRotationSlider_valueChanged(int value);

  void on_zRotationSpinBox_valueChanged(double arg1);

  void on_ScaleSlider_valueChanged(int value);

  void on_ScaleSpinBox_valueChanged(double arg1);

  void on_ScrenshotButton_clicked();

  void on_GifButton_clicked();

  void on_pushButton_clicked();

  void on_ModelColorButton_clicked();

  void on_perspectiveRadioButton_clicked(bool checked);

  void on_VertexColorButton_clicked();

  void on_FadeColorButton_clicked();

  void on_dottedRadioButton_clicked(bool checked);

  void on_LineWidthSlider_valueChanged(int value);

  void on_PointSizeSlider_valueChanged(int value);

  void on_EmptyRadioButton_toggled(bool checked);

  void on_CircleRadioButton_toggled(bool checked);

  void on_SquareRadioButton_toggled(bool checked);

  void on_SolidRadioButton_toggled(bool checked);

  void on_ParralelRadioButton_clicked(bool checked);

 private:
  struct TransformValues {
    int xTranslation;
    int yTranslation;
    int zTranslation;
    int xRotation;
    int yRotation;
    int zRotation;
    double scale;
  };

  TransformValues currentValues = {0, 0, 0, 0, 0, 0, 1.0};
  Ui::MainWindow *ui;
  object_t model;
  void transformAndDraw();
  bool recording;
  bool gifStart = 0, vertex = 1;
  int gifTime = 0;
  QGifImage *gifOut;
  QTimer *gifTimer;
  QColor modelColor;
  void startRecording();
  void stopRecording();
  void setColor(bool background, float r, float g, float b);
  QSettings settings;
  void saveSettings();
  void loadSettings();
  void initPanel();
  // Customization settings
  QColor edgeColor;
  QColor vertexColor;
  QColor faceColor;
  int edgeWidth;
  int vertexSize;
  QString projectionType;
  QString edgeType;
  QString vertexShape;
  QColor backgroundColor;
};
#endif  // MAINWINDOW_H
