#include <utils/utils.h>

#include <iostream>
#include <mutex>

#include <states/tor_cylinder_rotate_state/tor_cylinder_rotate_state.h>

#include <GL/glu.h>
#include <GL/glut.h>    // Include GLUT for cool drawing utils

#include <state_machine/state_machine.h>
#include <states/tor_cylinder_rotate_state/constants.h>
#include <states/tor_cylinder_state/constants.h>
#include <states/tor_stretch_cylinder_state/tor_stretch_cylinder_state.h>

using namespace states::tor_cylinder_rotate_state::constants;
using namespace states::tor_cylinder_state::constants;

namespace states {
  GLState* TorCylinderRotateState::display()
  {
    static GLuint textureID = utils::loadTexture(cone_texture_path);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear both color and depth buffers

    // --- Draw the solid cylinder ---
    glPushMatrix(); // Save the current matrix state

    // Create a quadric object for the cylinder
    GLUquadric* quadric = gluNewQuadric();

    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glTranslated(torus_start_pos.x, torus_start_pos.y, torus_start_pos.z);

    glutWireTorus(torus_inner_radius, torus_outer_radius, torus_sides, torus_rings);

    glPopMatrix(); // Restore the previous matrix state

    glPushMatrix(); // Save the current matrix state

    constexpr auto max_frames_count = tor_cylinder_rotate_state::constants::MAX_FRAMES_COUNT;

    frames_count = frames_count > max_frames_count ? max_frames_count : frames_count;

    glRotated(90 + frames_count * 360 / max_frames_count, 1, 0, 0);
    glTranslated(cylinder_start_pos.y, -cylinder_start_pos.x, cylinder_start_pos.z);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBindTexture(GL_TEXTURE_2D, textureID);

    // Draw the wireframe cylinder (same dimensions for the overlay)
    gluCylinder(quadric, cylinder_base, cylinder_top, cylinder_height, cylinder_slices, cylinder_stacks); // Same cylinder dimensions as the solid, increased slices

    glBindTexture(GL_TEXTURE_2D, 0);

    // Reset the polygon mode to solid rendering
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Clean up the quadric object
    gluDeleteQuadric(quadric);

    glPopMatrix(); // Restore the previous matrix state

    return this;
  }

  void TorCylinderRotateState::timeout()
  {
    std::cout << CURRENT_FUNCTION << " timeout " << frames_count << std::endl;
    frames_count++;
    if (frames_count == ::states::tor_cylinder_rotate_state::constants::MAX_FRAMES_COUNT)
      state_machine::StateMachine::instance()->set_state(new states::TorStretchCylinderState());
  }
} // namespace states

