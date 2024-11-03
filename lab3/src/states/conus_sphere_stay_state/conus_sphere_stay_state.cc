#include <states/conus_sphere_stay_state/conus_sphere_stay_state.h>

#include <iostream>
#include <cmath>

#include <GL/glut.h> // Include GLUT for cool drawing utils

#include <glwidget/glwidget.h>
#include <state_machine/state_machine.h>
#include <states/conus_sphere_stay_state/constants.h>
#include <states/tor_cylinder_state/constants.h>
#include <utils/utils.h>

using namespace states::conus_sphere_stay_state::constants;
using namespace states::tor_cylinder_state::constants;

namespace {
  void shadowMatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4])
  {
    GLfloat dot = groundplane[0] * lightpos[0] + groundplane[1] * lightpos[1] +
                  groundplane[2] * lightpos[2] + groundplane[3] * lightpos[3];

    shadowMat[0][0] = dot - lightpos[0] * groundplane[0];
    shadowMat[1][0] = 0.0f - lightpos[0] * groundplane[1];
    shadowMat[2][0] = 0.0f - lightpos[0] * groundplane[2];
    shadowMat[3][0] = 0.0f - lightpos[0] * groundplane[3];

    shadowMat[0][1] = 0.0f - lightpos[1] * groundplane[0];
    shadowMat[1][1] = dot - lightpos[1] * groundplane[1];
    shadowMat[2][1] = 0.0f - lightpos[1] * groundplane[2];
    shadowMat[3][1] = 0.0f - lightpos[1] * groundplane[3];

    shadowMat[0][2] = 0.0f - lightpos[2] * groundplane[0];
    shadowMat[1][2] = 0.0f - lightpos[2] * groundplane[1];
    shadowMat[2][2] = dot - lightpos[2] * groundplane[2];
    shadowMat[3][2] = 0.0f - lightpos[2] * groundplane[3];

    shadowMat[0][3] = 0.0f - lightpos[3] * groundplane[0];
    shadowMat[1][3] = 0.0f - lightpos[3] * groundplane[1];
    shadowMat[2][3] = 0.0f - lightpos[3] * groundplane[2];
    shadowMat[3][3] = dot - lightpos[3] * groundplane[3];
  }

  void findPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
  {
    GLfloat vec0[3], vec1[3];

    // Calculate two vectors from the three points
    vec0[0] = v1[0] - v0[0];
    vec0[1] = v1[1] - v0[1];
    vec0[2] = v1[2] - v0[2];

    vec1[0] = v2[0] - v0[0];
    vec1[1] = v2[1] - v0[1];
    vec1[2] = v2[2] - v0[2];

    // Compute the cross product to get the normal
    plane[0] = vec0[1] * vec1[2] - vec0[2] * vec1[1];
    plane[1] = vec0[2] * vec1[0] - vec0[0] * vec1[2];
    plane[2] = vec0[0] * vec1[1] - vec0[1] * vec1[0];

    // Normalize the plane normal
    GLfloat length = std::sqrt(plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2]);
    plane[0] /= length;
    plane[1] /= length;
    plane[2] /= length;

    // Calculate D
    plane[3] = -(plane[0] * v0[0] + plane[1] * v0[1] + plane[2] * v0[2]);
  }

} // namespace

namespace states {
  GLState* ConusSphereStayState::display()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the camera position and orientation in the model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(constants::CAMERA_POS_X, constants::CAMERA_POS_Y,
              constants::CAMERA_POS_Z,                                          // Camera position
              constants::LOOK_AT_X, constants::LOOK_AT_Y, constants::LOOK_AT_Z, // Look-at point
              constants::UP_X, constants::UP_Y, constants::UP_Z);               // Up vector

    // --- Draw Backdrop Rectangle (as a background plane) ---

    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.2f); // Light gray color for visibility

    glBegin(GL_QUADS);
    glVertex3f(-100.0f, -100.0f, 0.5f); // Bottom-left
    glVertex3f(100.0f, -100.0f, 0.5f);  // Bottom-right
    glVertex3f(100.0f, 100.0f, 0.5f);   // Top-right
    glVertex3f(-100.0f, 100.0f, 0.5f);  // Top-left
    glEnd();
    glPopMatrix();

    // --- Draw Opaque Objects ---

    // Draw the sphere
    glPushMatrix();
    glTranslated(sphere_start_pos.x, sphere_start_pos.y, sphere_start_pos.z);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Fully opaque
    glMaterialfv(GL_FRONT, GL_SPECULAR, states::conus_sphere_stay_state::constants::mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, states::conus_sphere_stay_state::constants::mat_shininess);
    glutSolidSphere(sphere_radius, sphere_slices, sphere_stacks);
    glPopMatrix();

    // --- Draw Textured Cylinder ---
    glPushMatrix();
    static GLuint textureID = utils::loadTexture(cone_texture_path);
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
    gluCylinder(quadric, cylinder_base, cylinder_top, cylinder_height, cylinder_slices,
                cylinder_stacks);
    glBindTexture(GL_TEXTURE_2D, 0);
    gluDeleteQuadric(quadric);
    glPopMatrix();

    // --- Draw Transparent Cone ---

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glPushMatrix();
    glTranslated(cone_start_pos.x, cone_start_pos.y, cone_start_pos.z);
    glRotated(90, -1, 0, 0);
    glColor4f(1.0f, 1.0f, 1.0f, constants::LOWER_ALPHA);
    GLfloat mat_diffuse_with_alpha[] = {1.0f, 1.0f, 1.0f, constants::LOWER_ALPHA};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_with_alpha);
    glutSolidCone(cone_base, cone_height, cone_slices, cone_stacks);
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
