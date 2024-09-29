#include <states/states.h>

#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>
#include "states.h"

void states::GLState::keyboard_callback(unsigned char key, int param1, int param2)
{
  if (key == 'q') {
    std::cout << "Quitting application";
    glutDestroyWindow(param1);
    exit(0);
  }
}

void states::GLState::reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}