#include "glwidget.h"

#include <iostream>

#include <GL/glut.h> // Include GLUT for cool drawing utils

#include <QWindow>
#include <QOpenGLContext>

#include <states/conus_sphere_stay_state/conus_sphere_stay_state.h>
#include <constants.h>

static glwidget::GLWidget* This = nullptr;

namespace glwidget {
  GLWidget::GLWidget(QWidget* parent)
    : QGLWidget(parent)
  {
    This = this;
  }

  void GLWidget::initializeGL()
  {
    // Initialize display mode for double buffering, color, and depth buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(constants::FOV_Y,
                   (double)constants::WINDOW_WIDTH / (double)constants::WINDOW_HEIGHT,
                   constants::NEAR_PLANE, constants::FAR_PLANE);

    // Switch back to the model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Reset the model-view matrix to identity

    // Enable depth testing for correct z-ordering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // Less than or equal for depth test

    // Enable face culling (optional, for performance; disable if you want to see both sides)
    glDisable(GL_CULL_FACE);

    // Enable blending if you need transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable lighting and the first light source
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Enable textures (optional)
    glEnable(GL_TEXTURE_2D);

    // Start timer for updates (assuming an update function is called per frame)
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

  std::array<GLfloat, 4> glwidget::GLWidget::get_light_position() { return This->light_position; }

} // namespace glwidget
