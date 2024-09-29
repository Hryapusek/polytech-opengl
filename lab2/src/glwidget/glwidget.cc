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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    startTimer(1000 / constants::FRAMES_PER_SECONDS);
  }

  void GLWidget::paintGL()
  {
    paint_light();
    state_machine_obj->display();
  }

  void GLWidget::resizeGL(int width, int height) { state_machine_obj->reshape(width, height); }

  void GLWidget::timerEvent(QTimerEvent* event)
  {
    state_machine_obj->timeout();
    update();
    QGLWidget::timerEvent(event);
  }

  void GLWidget::set_light_position(glm::fvec3 position)
  {
    light_position[0] = position.x;
    light_position[1] = position.y;
    light_position[2] = position.z;
  }

  void GLWidget::paint_light()
  {
    std::array<GLfloat, 4> intensity_light_ambient = {};
    std::array<GLfloat, 4> intensity_light_diffuse = {};
    std::array<GLfloat, 4> intensity_light_specular = {};
    std::array<GLfloat, 4> intensity_light_position = {};

    std::transform(light_ambient.begin(), light_ambient.end(), intensity_light_ambient.begin(),
                   [this](GLfloat val) { return val * light_intensity; });
    std::transform(light_diffuse.begin(), light_diffuse.end(), intensity_light_diffuse.begin(),
                   [this](GLfloat val) { return val * light_intensity; });
    std::transform(light_specular.begin(), light_specular.end(), intensity_light_specular.begin(),
                   [this](GLfloat val) { return val * light_intensity; });
    std::transform(light_position.begin(), light_position.end(), intensity_light_position.begin(),
                   [this](GLfloat val) { return val * light_intensity; });

    glLightfv(GL_LIGHT0, GL_AMBIENT, intensity_light_ambient.data());   // Ambient light
    glLightfv(GL_LIGHT0, GL_DIFFUSE, intensity_light_diffuse.data());   // Diffuse light
    glLightfv(GL_LIGHT0, GL_SPECULAR, intensity_light_specular.data()); // Specular light
    glLightfv(GL_LIGHT0, GL_POSITION, intensity_light_position.data());
  }

} // namespace glwidget
