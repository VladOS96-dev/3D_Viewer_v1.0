#ifndef WIDGET_H
#define WIDGET_H
extern "C" {
#include "obj_parcer.h"
}

#include <QColor>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <vector>
class Widget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

  void draw_obj(const char *filename);
  void draw(object_t model);
  void setupShader();
  void clearCanvas();
  object_t getModel() const;
 public slots:
  void setProjectionType(const QString &type);
  void setVertexColor(const QColor &color);
  void setEdgeColor(const QColor &color);
  void setModelColor(const QColor &color);
  void setBackgroundColor(const QColor &color);
  void setEdgeType(const QString &type);
  void setEdgeWidth(int width);
  void setVertexSize(int size);
  void setVertexShape(const QString &shape);

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void initializeVertices();
  void initializeEdges();

 private:
  object_t model;

  GLuint vao;
  GLuint vboVertices;
  GLuint vboTexCoords;
  GLuint vboIndices;
  GLuint shaderProgram;
  int numIndices;

  QColor modelColor;
  QColor backgroundColor;
  QColor vertexColor;
  QColor faceColor;
  QColor edgeColor;
  QString projectionType;
  QString edgeType;
  QString vertexShape;
  int edgeWidth;
  int vertexSize;
  void setupProjectionMatrix();
  void drawEdges();
  void drawVertices();
};

#endif  // WIDGET_H
