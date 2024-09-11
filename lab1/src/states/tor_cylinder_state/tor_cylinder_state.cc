#include <states/tor_cylinder_state/tor_cylinder_state.h>

#include <iostream>

#include <GL/glu.h>
#include <GL/glut.h>    // Include GLUT for cool drawing utils
#include <GLFW/glfw3.h> // Include GLFW for window and context management

#include <state_machine/state_machine.h>
#include <states/tor_cylinder_state/constants.h>

using namespace states::tor_cylinder_state::constants;

namespace states {
  GLState* TorCylinderState::display()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear both color and depth buffers

    // --- Draw the solid cylinder ---
    glPushMatrix(); // Save the current matrix state

    // Create a quadric object for the cylinder
    GLUquadric* quadric = gluNewQuadric();

    glTranslated(torus_start_pos.x, torus_start_pos.y, torus_start_pos.z);

    glutWireTorus(torus_inner_radius, torus_outer_radius, torus_sides, torus_rings);

    // --- Draw the wireframe cylinder on top ---
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Switch to wireframe mode

    // Set the color for the wireframe
    glColor3f(0.0, 1.0, 0.0); // Green color for the wireframe lines

    glPopMatrix(); // Restore the previous matrix state

    glPushMatrix(); // Save the current matrix state

    glRotated(90, 1, 0, 0);
    glTranslated(cylinder_start_pos.y, -cylinder_start_pos.x, cylinder_start_pos.z);

    // Draw the wireframe cylinder (same dimensions for the overlay)
    gluCylinder(quadric, cylinder_base, cylinder_top, cylinder_height, cylinder_slices, cylinder_stacks); // Same cylinder dimensions as the solid, increased slices

    // Reset the polygon mode to solid rendering
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Clean up the quadric object
    gluDeleteQuadric(quadric);

    glPopMatrix(); // Restore the previous matrix state

    // Swap the buffers to display the rendered frame
    glutSwapBuffers();

    return this;
  }

  void TorCylinderState::timeout()
  {
    std::cout << __func__ << " timeout " << frames_count << std::endl;
    frames_count++;
    glutPostRedisplay();
    // if (frames_count == MAX_FRAMES_COUNT)
    //   state_machine::StateMachine::instance()->set_state(new states::ConusSphereMoveState());
  }
} // namespace states
