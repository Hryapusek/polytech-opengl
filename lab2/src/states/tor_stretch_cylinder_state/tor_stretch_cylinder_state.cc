#include <states/tor_stretch_cylinder_state/tor_stretch_cylinder_state.h>

#include <iostream>
#include <cmath>

#include <GL/glu.h>
#include <GL/glut.h>    // Include GLUT for cool drawing utils

#include <state_machine/state_machine.h>
#include <states/tor_cylinder_state/constants.h>
#include <states/tor_stretch_cylinder_state/constants.h>
#include <states/conus_sphere_stay_state/conus_sphere_stay_state.h>

#include <utils/utils.h>

using namespace states::tor_cylinder_state::constants;
using namespace states::tor_stretch_cylinder_state::constants;

namespace states {
  GLState* TorStretchCylinderState::display()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear both color and depth buffers

    // --- Draw the solid cylinder ---
    glPushMatrix(); // Save the current matrix state
    glLoadIdentity();

    // Create a quadric object for the cylinder
    GLUquadric* quadric = gluNewQuadric();

    frames_count = frames_count > tor_stretch_cylinder_state::constants::MAX_FRAMES_COUNT ? tor_stretch_cylinder_state::constants::MAX_FRAMES_COUNT : frames_count;

    constexpr auto max_frames = tor_stretch_cylinder_state::constants::MAX_FRAMES_COUNT;

    glScaled(torus_result_scale.x, torus_result_scale.y - (1.0 * max_frames - frames_count) / max_frames, 1);
    glTranslated(torus_start_pos.x, torus_start_pos.y, torus_start_pos.z);

    glutWireTorus(torus_inner_radius, torus_outer_radius, torus_sides, torus_rings);

    // --- Draw the wireframe cylinder on top ---
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Switch to wireframe mode

    // Set the color for the wireframe
    glColor3f(0.0, 1.0, 0.0); // Green color for the wireframe lines

    glPopMatrix(); // Restore the previous matrix state

    glPushMatrix(); // Save the current matrix state

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glRotated(90, 1, 0, 0);
    glTranslated(cylinder_start_pos.y, -cylinder_start_pos.x, cylinder_start_pos.z);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Draw the wireframe cylinder (same dimensions for the overlay)
    gluCylinder(quadric, cylinder_base, cylinder_top, cylinder_height, cylinder_slices, cylinder_stacks); // Same cylinder dimensions as the solid, increased slices

    // Reset the polygon mode to solid rendering
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Clean up the quadric object
    gluDeleteQuadric(quadric);

    glPopMatrix(); // Restore the previous matrix state

    // Swap the buffers to display the rendered frame

    return this;
  }

  void TorStretchCylinderState::timeout()
  {
    std::cout << CURRENT_FUNCTION << " timeout " << frames_count << std::endl;
    frames_count++;
    if (frames_count == tor_stretch_cylinder_state::constants::MAX_FRAMES_COUNT)
      state_machine::StateMachine::instance()->set_state(new states::ConusSphereStayState());
  }
} // namespace states

