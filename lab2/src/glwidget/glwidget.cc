#include "glwidget.h"

#include <iostream>

#include <GL/glut.h> // Include GLUT for cool drawing utils

#include <QWindow>
#include <QOpenGLContext>

#include <states/conus_sphere_stay_state/conus_sphere_stay_state.h>
#include <constants.h>

namespace glwidget {
  GLWidget::GLWidget(QWidget* parent)
    : QGLWidget(parent)
  {
  }

  void GLWidget::initializeGL()
  {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glMatrixMode(GL_PROJECTION); // Switch to projection matrix
    glLoadIdentity();            // Reset the projection matrix

    // Set up a perspective projection using the constants
    gluPerspective(constants::FOV_Y,
                   (double)constants::WINDOW_WIDTH / (double)constants::WINDOW_HEIGHT,
                   constants::NEAR_PLANE, constants::FAR_PLANE);

    // Set the camera position and look-at point using the constants
    gluLookAt(constants::CAMERA_POS_X, constants::CAMERA_POS_Y,
              constants::CAMERA_POS_Z,                                          // Camera position
              constants::LOOK_AT_X, constants::LOOK_AT_Y, constants::LOOK_AT_Z, // Look-at point
              constants::UP_X, constants::UP_Y, constants::UP_Z);               // Up vector

    glMatrixMode(GL_MODELVIEW); // Switch back to model-view matrix
    glLoadIdentity();           // Reset the model-view matrix

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glDisable(GL_CULL_FACE); // Disable face culling
    startTimer(1000 / constants::FRAMES_PER_SECONDS);
  }

  void GLWidget::paintGL() { 

    state_machine_obj->display(); }

  void GLWidget::resizeGL(int width, int height) { state_machine_obj->reshape(width, height); }

  void glwidget::GLWidget::timerEvent(QTimerEvent* event)
  {
    state_machine_obj->timeout();
    update();
    QGLWidget::timerEvent(event);
  }



} // namespace glwidget
