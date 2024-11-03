#pragma once

#include <QGLWidget>
#include <QWidget>

#include <glm/glm.hpp>

#include <state_machine/state_machine.h>

namespace glwidget {
  class GLWidget : public QGLWidget
  {
   public:
    GLWidget(QWidget* parent);

    virtual ~GLWidget() = default;
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void timerEvent(QTimerEvent* event) override;

   public slots:
    void set_light_position(glm::fvec3 position);
    void set_light_position_x(GLfloat x) {
      light_position[0] = x;
    }
    void set_light_position_y(GLfloat y) {
      light_position[1] = y;
    }
    void set_light_position_z(GLfloat z) {
      light_position[2] = z;
    }

    void set_light_color(QColor color) {
      light_ambient[0] = color.redF();
      light_ambient[1] = color.greenF();
      light_ambient[2] = color.blueF();
    }

    void set_light_intensity(GLfloat new_intensity) {
      light_intensity = new_intensity;
    }

    static std::array<GLfloat, 4> get_light_position();

   protected:
    state_machine::StateMachine* state_machine_obj = state_machine::StateMachine::instance();
    std::array<GLfloat, 4> light_position = {1.0f, 1.0f, 1.0f, 1.0f};

    std::array<GLfloat, 4> light_ambient = { 1.f, 1.f, 1.f, 1.0f }; // Low intensity ambient light
    std::array<GLfloat, 4> light_diffuse = { 1.0f, 1.0f, 1.0f, 1.0f }; // Full intensity white diffuse light
    std::array<GLfloat, 4> light_specular = { 1.0f, 1.0f, 1.0f, 1.0f }; // Full intensity white specular light

    GLfloat light_intensity = 1.0f;

    void paint_light();
  };
} // namespace glwidget
