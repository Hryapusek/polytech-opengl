#include <states/conus_sphere_moving_state/conus_sphere_moving_state.h>

#include <iostream>

#include <GL/glut.h> // Include GLUT for cool drawing utils

#include <state_machine/state_machine.h>
#include <states/conus_sphere_stay_state/constants.h>
#include <states/conus_sphere_moving_state/constants.h>
#include <states/tor_cylinder_state/tor_cylinder_state.h>

using namespace states::conus_sphere_moving_state::constants;
using namespace states::conus_sphere_stay_state::constants;

namespace states {
  GLState* ConusSphereMovingState::display()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear both color and depth buffers
    glPushMatrix();
    glLoadIdentity();
    frames_count = frames_count <= MAX_ANIMATION_FRAMES ? frames_count : MAX_ANIMATION_FRAMES;
    glm::dvec3 new_cone_pos {
      cone_start_pos.x + (cone_end_pos.x - cone_start_pos.x) / MAX_ANIMATION_FRAMES * frames_count,
      cone_start_pos.y + (cone_end_pos.y - cone_start_pos.y) / MAX_ANIMATION_FRAMES * frames_count,
      cone_start_pos.z + (cone_end_pos.z - cone_start_pos.z) / MAX_ANIMATION_FRAMES * frames_count,
    };

    glTranslated(new_cone_pos.x, new_cone_pos.y, new_cone_pos.z);

    glColor4f(1.0f, 1.0f, 1.0f, constants::LOWER_ALPHA);

    glRotated(90, -1, 0, 0);
    glutWireCone(cone_base, cone_height, cone_slices, cone_stacks);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glPopMatrix();
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();

    glm::dvec3 new_sphere_pos {
      sphere_start_pos.x +
        (sphere_end_pos.x - sphere_start_pos.x) / MAX_ANIMATION_FRAMES * frames_count,
      sphere_start_pos.y +
        (sphere_end_pos.y - sphere_start_pos.y) / MAX_ANIMATION_FRAMES * frames_count,
      sphere_start_pos.z +
        (sphere_end_pos.z - sphere_start_pos.z) / MAX_ANIMATION_FRAMES * frames_count,
    };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glTranslated(new_sphere_pos.x, new_sphere_pos.y, new_sphere_pos.z);
    glutSolidSphere(sphere_radius, sphere_slices, sphere_stacks);

    glPopMatrix();
    return this;
  }

  void ConusSphereMovingState::timeout()
  {
    std::cout << "ConusSphereMovingState timeout " << frames_count << std::endl;
    frames_count++;
    if (frames_count == MAX_FRAMES_COUNT) {
      state_machine::StateMachine::instance()->set_state(new states::TorCylinderState());
    }
  }
} // namespace states
