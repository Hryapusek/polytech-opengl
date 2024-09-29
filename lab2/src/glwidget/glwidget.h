#pragma once

#include <QGLWidget>
#include <QWidget>

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

   protected:
    state_machine::StateMachine* state_machine_obj = state_machine::StateMachine::instance();
  };
} // namespace glwidget
