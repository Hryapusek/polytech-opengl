#include <iostream>

#include <GL/glut.h>    // Include GLUT for cool drawing utils

#include <states/conus_sphere_stay_state/conus_sphere_stay_state.h>
#include <state_machine/state_machine.h>
#include <constants.h>

state_machine::StateMachine *state_machine_obj = state_machine::StateMachine::instance();

void init()
{
    glMatrixMode(GL_PROJECTION);  // Switch to projection matrix
    glLoadIdentity();             // Reset the projection matrix

    // Set up a perspective projection using the constants
    gluPerspective(constants::FOV_Y, 
                   (double)constants::WINDOW_WIDTH / (double)constants::WINDOW_HEIGHT, 
                   constants::NEAR_PLANE, 
                   constants::FAR_PLANE);

    // Set the camera position and look-at point using the constants
    gluLookAt(constants::CAMERA_POS_X, constants::CAMERA_POS_Y, constants::CAMERA_POS_Z,  // Camera position
              constants::LOOK_AT_X, constants::LOOK_AT_Y, constants::LOOK_AT_Z,          // Look-at point
              constants::UP_X, constants::UP_Y, constants::UP_Z);                        // Up vector

    glMatrixMode(GL_MODELVIEW);   // Switch back to model-view matrix
    glLoadIdentity();             // Reset the model-view matrix

    glEnable(GL_DEPTH_TEST);      // Enable depth testing
    glDisable(GL_CULL_FACE);      // Disable face culling
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
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  init();

  glutInitWindowSize(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT);
  glutInitWindowPosition(constants::WINDOW_POSITION_X, constants::WINDOW_POSITION_Y);
  
  glutCreateWindow("Simple GLUT App with animations");

  state_machine_obj->set_state(new states::ConusSphereStayState());

  glutTimerFunc(1000 / constants::FRAMES_PER_SECONDS, timeout, 0);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard_callback);

  glutMainLoop();
  return 0;
}
