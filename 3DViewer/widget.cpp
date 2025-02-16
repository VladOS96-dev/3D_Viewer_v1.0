#include "widget.h"

#include <math.h>

#include <QDebug>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include "mainwindow.h"

Widget::Widget(QWidget* parent)
    : QOpenGLWidget(parent),
      modelColor(Qt::black),
      backgroundColor(Qt::white),
      vertexColor(Qt::black),
      faceColor(Qt::black),
      edgeColor(Qt::black),
      projectionType("Parallel"),
      edgeType("Solid"),
      vertexShape("None"),
      edgeWidth(1),
      vertexSize(1) {}

Widget::~Widget() {
  makeCurrent();
  glDeleteBuffers(1, &vboVertices);
  glDeleteBuffers(1, &vboIndices);

  glDeleteProgram(shaderProgram);
  doneCurrent();
}
void Widget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  setupShader();
}

object_t Widget::getModel() const { return model; }
void Widget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void Widget::paintGL() {
  glClearColor(backgroundColor.redF(), backgroundColor.greenF(),
               backgroundColor.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (model.num_vertices != 0) {
    glUseProgram(shaderProgram);
    setupProjectionMatrix();

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < model.num_faces; ++i) {
      const face_t& face = model.faces[i];
      for (int j = 0; j < model.faces[i].num_indices_in_face; j++) {
        const vertex_t& v = model.points[face.vertex_indices[j] - 1];
        glVertex3f(v.x, v.y, v.z);
      }
    }
    glEnd();

    drawEdges();
    drawVertices();

    glUseProgram(0);
  }
}

void Widget::setupProjectionMatrix() {
  QMatrix4x4 projectionMatrix;
  if (projectionType == "Perspective") {
    projectionMatrix.perspective(
        45.0f, static_cast<float>(width()) / static_cast<float>(height()), 0.1f,
        100.0f);

  } else {
    projectionMatrix.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
  }

  GLint mvpLocation = glGetUniformLocation(shaderProgram, "u_MVP");
  glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, projectionMatrix.constData());

  QMatrix4x4 modelViewMatrix;
  modelViewMatrix.setToIdentity();
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_ModelView"), 1,
                     GL_FALSE, modelViewMatrix.constData());
  glUniform4f(glGetUniformLocation(shaderProgram, "u_Color"), modelColor.redF(),
              modelColor.greenF(), modelColor.blueF(), 1.0f);
}

void Widget::drawEdges() {
  glUniform4f(glGetUniformLocation(shaderProgram, "u_Color"), edgeColor.redF(),
              edgeColor.greenF(), edgeColor.blueF(), 1.0f);
  glLineWidth(edgeWidth);
  if (edgeType == "Dashed") {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  glBegin(GL_LINES);
  for (int i = 0; i < model.num_faces; ++i) {
    const face_t& face = model.faces[i];
    if (model.points != NULL && face.num_indices_in_face != 0) {
      const vertex_t& v1 = model.points[face.vertex_indices[0] - 1];
      const vertex_t& v2 = model.points[face.vertex_indices[1] - 1];
      const vertex_t& v3 = model.points[face.vertex_indices[2] - 1];
      glVertex3f(v1.x, v1.y, v1.z);
      glVertex3f(v2.x, v2.y, v2.z);
      glVertex3f(v2.x, v2.y, v2.z);
      glVertex3f(v3.x, v3.y, v3.z);
      glVertex3f(v1.x, v1.y, v1.z);
    }
  }
  glEnd();
  glDisable(GL_LINE_STIPPLE);
}

void Widget::drawVertices() {
  glUniform4f(glGetUniformLocation(shaderProgram, "u_Color"),
              vertexColor.redF(), vertexColor.greenF(), vertexColor.blueF(),
              1.0f);

  if (vertexShape == "Circle") {
    glPointSize(vertexSize);
    glBegin(GL_POINTS);
    for (int i = 0; i < model.num_vertices; ++i) {
      const vertex_t& vertex = model.points[i];

      glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
  } else if (vertexShape == "Square") {
    glBegin(GL_QUADS);
    for (int i = 0; i < model.num_vertices; ++i) {
      const vertex_t& vertex = model.points[i];
      float halfSize = vertexSize * 0.005f;
      glVertex3f(vertex.x - halfSize, vertex.y - halfSize, vertex.z);
      glVertex3f(vertex.x + halfSize, vertex.y - halfSize, vertex.z);
      glVertex3f(vertex.x + halfSize, vertex.y + halfSize, vertex.z);
      glVertex3f(vertex.x - halfSize, vertex.y + halfSize, vertex.z);
    }
    glEnd();
  }
}

void Widget::draw_obj(const char* filename) {
  parse_obj_file(filename, &model);
}
void Widget::draw(object_t model) {
  this->model = model;
  update();
}

void Widget::setupShader() {
  // Vertex shader source code
  const char* vertexShaderSource = R"(
    #version 120
    attribute vec3 position;
    uniform mat4 u_MVP;
    void main(void) {
        gl_Position = u_MVP * vec4(position, 1.0);
    }
)";

  // Fragment shader source code
  const char* fragmentShaderSource = R"(
    #version 120
    uniform vec4 u_Color;
    void main(void) {
        gl_FragColor = u_Color;
    }
)";

  // Compile vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Check for vertex shader compile errors
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
  }

  // Compile fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Check for fragment shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
  }

  // Create shader program
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check for shader linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
  }

  // Delete shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Use the shader program and set the color to blue
  glUseProgram(shaderProgram);
  GLint colorLocation = glGetUniformLocation(shaderProgram, "u_Color");
  if (colorLocation != -1) {
    glUniform4f(colorLocation, 0.0f, 0.0f, 1.0f, 1.0f);  // Set color to blue
  } else {
    qDebug() << "ERROR::SHADER::UNIFORM::COLOR_LOCATION_NOT_FOUND";
  }

  // Set MVP matrix (for simplicity, we use identity matrix)
  GLint mvpLocation = glGetUniformLocation(shaderProgram, "u_MVP");
  if (mvpLocation != -1) {
    QMatrix4x4 mvp;
    mvp.setToIdentity();
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, mvp.constData());
  } else {
    qDebug() << "ERROR::SHADER::UNIFORM::MVP_LOCATION_NOT_FOUND";
  }
}
void Widget::setProjectionType(const QString& type) {
  projectionType = type;
  update();  // Ensure the widget is repainted
}

void Widget::setVertexColor(const QColor& color) {
  vertexColor = color;
  update();  // Ensure the widget is repainted
}

void Widget::setEdgeColor(const QColor& color) {
  edgeColor = color;
  update();  // Ensure the widget is repainted
}

void Widget::setEdgeType(const QString& type) {
  edgeType = type;
  update();  // Ensure the widget is repainted
}

void Widget::setEdgeWidth(int width) {
  edgeWidth = width;
  update();  // Ensure the widget is repainted
}

void Widget::setVertexSize(int size) {
  vertexSize = size;
  update();  // Ensure the widget is repainted
}

void Widget::setVertexShape(const QString& shape) {
  vertexShape = shape;
  update();  // Ensure the widget is repainted
}
void Widget::clearCanvas() {
  makeCurrent();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  doneCurrent();
}
void Widget::setBackgroundColor(const QColor& color) {
  backgroundColor = color;
  update();  // Ensure the widget is repainted
}
void Widget::setModelColor(const QColor& color) {
  modelColor = color;
  update();  // Ensure the widget is repainted
}
