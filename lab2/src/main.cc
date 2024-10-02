#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include <QApplication>

#include <main_window/main_window.h>
#include <state_machine/state_machine.h>
#include <states/conus_sphere_stay_state/conus_sphere_stay_state.h>
#include <constants.h>

state_machine::StateMachine *state_machine_obj = state_machine::StateMachine::instance();

//Запросить режим двойной буферизации
//Зарегистрировать функции обработки мышиного ввода
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glewInit();
  QApplication app(argc, argv);
  main_window_ns::MainWindow main_window;
  main_window.resize(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT);
  state_machine_obj->set_state(new states::ConusSphereStayState());
  main_window.show();
  return app.exec();
}
