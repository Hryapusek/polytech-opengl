#include "glwidget.h"
#include <QOpenGLShader>
#include <QMatrix4x4>

namespace glwidget {
  GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
  {
    connect(&timer, &QTimer::timeout, this, QOverload<>::of(&GLWidget::update));
    timer.start(16); // Roughly 60 FPS
  }

  void GLWidget::initializeGL()
  {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();
  }

  void GLWidget::resizeGL(int width, int height)
  {
    projection.setToIdentity();
    projection.perspective(45.0f, float(width) / height, 0.1f, 100.0f);
  }

  void GLWidget::paintGL()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();

    QMatrix4x4 model;
    model.rotate(angle, 0.0f, 1.0f, 0.0f); // Rotate cube over time
    angle += 1.0f;                         // Increment angle for rotation

    QMatrix4x4 view;
    view.lookAt(QVector3D(0.0f, 0.0f, 5.0f), QVector3D(0.0f, 0.0f, 0.0f),
                QVector3D(0.0f, 1.0f, 0.0f));

    shaderProgram.setUniformValue("model", model);
    shaderProgram.setUniformValue("view", view);
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("lightPos", QVector3D(2.0f, 2.0f, 2.0f));     // Move light closer
    shaderProgram.setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 5.0f));      // Camera position
    shaderProgram.setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));   // White light
    shaderProgram.setUniformValue("objectColor", QVector3D(1.0f, 0.5f, 0.31f)); // Orange object

    drawCube();

    shaderProgram.release();
  }

  void GLWidget::initShaders()
  {
    // Load vertex and fragment shaders
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/shadow.vert"))
      qWarning() << "Vertex shader compilation error:" << shaderProgram.log();

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/shadow.frag"))
      qWarning() << "Fragment shader compilation error:" << shaderProgram.log();

    // Link the shader program
    if (!shaderProgram.link())
      qWarning() << "Shader program linking error:" << shaderProgram.log();
  }

  void GLWidget::drawCube()
  {
    // Define vertices and normals for a cube
    static const GLfloat vertices[] = {
      // Positions           // Normals
      -1.0f, -1.0f, -1.0f, 0.0f,  0.0f,  -1.0f, // Back face
      1.0f,  -1.0f, -1.0f, 0.0f,  0.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
      0.0f,  0.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,

      -1.0f, -1.0f, 1.0f,  0.0f,  0.0f,  1.0f, // Front face
      1.0f,  -1.0f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f,
      0.0f,  0.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  0.0f,  0.0f,  1.0f,

      -1.0f, 1.0f,  1.0f,  -1.0f, 0.0f,  0.0f, // Left face
      -1.0f, 1.0f,  -1.0f, -1.0f, 0.0f,  0.0f,  -1.0f, -1.0f, -1.0f,
      -1.0f, 0.0f,  0.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 0.0f,  0.0f,

      1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, // Right face
      1.0f,  1.0f,  -1.0f, 1.0f,  0.0f,  0.0f,  1.0f,  -1.0f, -1.0f,
      1.0f,  0.0f,  0.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  0.0f,  0.0f,

      -1.0f, -1.0f, -1.0f, 0.0f,  -1.0f, 0.0f, // Bottom face
      1.0f,  -1.0f, -1.0f, 0.0f,  -1.0f, 0.0f,  1.0f,  -1.0f, 1.0f,
      0.0f,  -1.0f, 0.0f,  -1.0f, -1.0f, 1.0f,  0.0f,  -1.0f, 0.0f,

      -1.0f, 1.0f,  -1.0f, 0.0f,  1.0f,  0.0f, // Top face
      1.0f,  1.0f,  -1.0f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.0f,
      0.0f,  1.0f,  0.0f,  -1.0f, 1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), vertices);
    glNormalPointer(GL_FLOAT, 6 * sizeof(GLfloat), vertices + 3);

    glDrawArrays(GL_QUADS, 0, 24); // Draw the cube

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
  }
} // namespace glwidget
