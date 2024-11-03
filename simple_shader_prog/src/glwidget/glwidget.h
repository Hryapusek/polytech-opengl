#pragma once

#include <QTimer>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <glm/glm.hpp>

#include <state_machine/state_machine.h>

namespace glwidget {
  class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
  {
   public:
    GLWidget(QWidget* parent = nullptr);
    ~GLWidget() override = default;

   protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

   private:
    QOpenGLShaderProgram shaderProgram;
    QMatrix4x4 projection;
    float angle = 0.0f;
    QTimer timer;

    void initShaders();
    void drawCube();
  };
} // namespace glwidget
