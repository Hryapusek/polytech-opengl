#include <states/conus_sphere_stay_state/conus_sphere_stay_state.h>

#include <iostream>

#include <GL/glut.h>    // Include GLUT for cool drawing utils

#include <state_machine/state_machine.h>
#include <states/conus_sphere_moving_state/conus_sphere_moving_state.h>
#include <states/conus_sphere_stay_state/constants.h>

using namespace states::conus_sphere_stay_state::constants;

namespace states {
  GLState* ConusSphereStayState::display()
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glLoadIdentity();

    glTranslated(cone_start_pos.x, cone_start_pos.y, cone_start_pos.z);

    glRotated(90, -1, 0, 0);
    glutWireCone(cone_base, cone_height, cone_slices, cone_stacks);

    glPopMatrix();
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();

    glTranslated(sphere_start_pos.x, sphere_start_pos.y, sphere_start_pos.z);
    glutWireSphere(sphere_radius, sphere_slices, sphere_stacks);

    glutSwapBuffers();
    glPopMatrix();
    return this;
  }

  void ConusSphereStayState::timeout()
  {
    std::cout << "ConusSphereStayState timeout " << frames_count << std::endl;
    frames_count++;
    glutPostRedisplay();
    if (frames_count == MAX_FRAMES_COUNT) {
      std::cout << "Switching the state\n";
      state_machine::StateMachine::instance()->set_state(new states::ConusSphereMovingState());
    }
  }
} // namespace states
