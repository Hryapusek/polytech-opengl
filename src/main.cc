#include <iostream>

#include <GL/glut.h>    // Include GLUT for cool drawing utils
#include <GLFW/glfw3.h> // Include GLFW for window and context management

#include <states/conus_sphere_stay_state/conus_sphere_stay_state.h>
#include <state_machine/state_machine.h>
#include <constants.h>

state_machine::StateMachine *state_machine_obj = state_machine::StateMachine::instance();

void init()
{
  glViewport(0, 0, constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT);
  glClearColor(0.0, 0.0, 0.0, 1);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(constants::ORTHO_LEFT, constants::ORTHO_RIGHT, constants::ORTHO_BOTTOM, constants::ORTHO_TOP, constants::ORTHO_NEAR, constants::ORTHO_FAR);
}

void display()
{
  state_machine_obj->display();
}

void reshape(int w, int h)
{
  state_machine_obj->reshape(w, h);
}

void keyboard_callback(unsigned char key, int param1, int param2)
{
  state_machine_obj->keyboard_callback(key, param1, param2);
}

void mouse(int button, int state, int x, int y)
{
  state_machine_obj->mouse(button, state, x, y);
}

void timeout(int /*unused*/)
{

  if (state_machine_obj->timeout())
    glutTimerFunc(1000 / constants::FRAMES_PER_SECONDS, timeout, 0);

}

//Запросить режим двойной буферизации
//Зарегистрировать функции обработки мышиного ввода
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  glutInitWindowSize(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT);
  glutInitWindowPosition(constants::WINDOW_POSITION_X, constants::WINDOW_POSITION_Y);
  
  glutCreateWindow("Simple GLUT App with animations");
  init();

  state_machine_obj->set_state(new states::ConusSphereStayState());

  glutTimerFunc(1000 / constants::FRAMES_PER_SECONDS, timeout, 0);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard_callback);

  glutMainLoop();
  return 0;
}
