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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear both color and depth buffers
    glColor4f(1.0f, 1.0f, 1.0f, constants::LOWER_ALPHA);
    glPushMatrix();
    glLoadIdentity();

    glTranslated(cone_start_pos.x, cone_start_pos.y, cone_start_pos.z);

    glRotated(90, -1, 0, 0);
    glutWireCone(cone_base, cone_height, cone_slices, cone_stacks);

    glPopMatrix();
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();

    glColor4f(1.0f, 1.0f, 1.0f, 1.f);

    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // White specular color
    GLfloat mat_shininess[] = { 50.0f }; // Shininess factor (higher values mean more polished)

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glTranslated(sphere_start_pos.x, sphere_start_pos.y, sphere_start_pos.z);
    glutSolidSphere(sphere_radius, sphere_slices, sphere_stacks); // Draw filled sphere

    glPopMatrix();
    return this;
  }

  void ConusSphereStayState::timeout()
  {
    std::cout << "ConusSphereStayState timeout " << frames_count << std::endl;
    frames_count++;
    if (frames_count == MAX_FRAMES_COUNT) {
      std::cout << "Switching the state\n";
      state_machine::StateMachine::instance()->set_state(new states::ConusSphereMovingState());
    }
  }
} // namespace states
