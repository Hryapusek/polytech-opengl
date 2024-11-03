#include <states/conus_sphere_stay_state/conus_sphere_stay_state.h>

#include <iostream>

#include <GL/glut.h> // Include GLUT for cool drawing utils

#include <state_machine/state_machine.h>
#include <states/conus_sphere_stay_state/constants.h>
#include <states/tor_cylinder_state/constants.h>
#include <utils/utils.h>

using namespace states::conus_sphere_stay_state::constants;
using namespace states::tor_cylinder_state::constants;

namespace states {
  GLState* ConusSphereStayState::display()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear both color and depth buffers

    // --- Draw Opaque Objects First ---

    glPushMatrix();
    glLoadIdentity();

    // Draw opaque sphere
    glTranslated(sphere_start_pos.x, sphere_start_pos.y, sphere_start_pos.z);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Fully opaque
    glMaterialfv(GL_FRONT, GL_SPECULAR, states::conus_sphere_stay_state::constants::mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, states::conus_sphere_stay_state::constants::mat_shininess);
    glutSolidSphere(sphere_radius, sphere_slices, sphere_stacks); // Draw filled sphere

    glPopMatrix();

    // --- Draw Textured Cylinder ---
    glPushMatrix(); // Save the current matrix state
    static GLuint textureID = utils::loadTexture(cone_texture_path);

    // Create a quadric object for the cylinder
    GLUquadric* quadric = gluNewQuadric();

    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glRotated(90, 1, 0, 0);
    glTranslated(cylinder_start_pos.y + 20, -cylinder_start_pos.x, cylinder_start_pos.z);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, states::tor_cylinder_state::constants::mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, states::tor_cylinder_state::constants::mat_shininess);

    glBindTexture(GL_TEXTURE_2D, textureID);

    // Draw the textured cylinder
    gluCylinder(quadric, cylinder_base, cylinder_top, cylinder_height, cylinder_slices,
                cylinder_stacks);

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

    // Clean up the quadric object
    gluDeleteQuadric(quadric);
    glPopMatrix(); // Restore the previous matrix state

    // --- Draw Transparent Cone Last ---

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable depth writing for the transparent object
    glDepthMask(GL_FALSE);

    glPushMatrix();
    glLoadIdentity();

    glTranslated(cone_start_pos.x, cone_start_pos.y, cone_start_pos.z);
    glRotated(90, -1, 0, 0);

    // Set color with transparency (for lighting)
    glColor4f(1.0f, 1.0f, 1.0f, constants::LOWER_ALPHA);

    // Set the material with transparency
    GLfloat mat_diffuse_with_alpha[] = {1.0f, 1.0f, 1.0f, constants::LOWER_ALPHA};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_with_alpha);

    // Draw the transparent cone
    glutSolidCone(cone_base, cone_height, cone_slices, cone_stacks);

    // Restore depth writing and disable blending
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glPopMatrix();

    return this;
  }

  void ConusSphereStayState::timeout()
  {
    std::cout << "ConusSphereStayState timeout " << frames_count << std::endl;
    frames_count++;
    // if (frames_count == MAX_FRAMES_COUNT) {
    //   std::cout << "Switching the state\n";
    //   state_machine::StateMachine::instance()->set_state(new states::ConusSphereMovingState());
    // }
  }
} // namespace states
