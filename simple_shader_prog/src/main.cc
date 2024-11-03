#include <iostream>

#include <QApplication>

#include <main_window/main_window.h>
#include <state_machine/state_machine.h>
#include <states/conus_sphere_cylinder_stay_state/conus_sphere_cylinder_stay_state.h>
#include <constants.h>

//Запросить режим двойной буферизации
//Зарегистрировать функции обработки мышиного ввода
int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  main_window_ns::MainWindow main_window;
  main_window.resize(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT);
  main_window.show();
  return app.exec();
}
