#pragma once

#include <stdio.h> // For size_t
#include <GL/gl.h>

namespace constants
{
  static constexpr size_t FRAMES_PER_SECONDS = 60;
  static constexpr GLdouble ORTHO_LEFT = -100;
  static constexpr GLdouble ORTHO_RIGHT = 100;
  static constexpr GLdouble ORTHO_BOTTOM = -100;
  static constexpr GLdouble ORTHO_TOP = 100;
  static constexpr GLdouble ORTHO_NEAR = -100;
  static constexpr GLdouble ORTHO_FAR = 100;

  static constexpr size_t WINDOW_WIDTH = 800;
  static constexpr size_t WINDOW_HEIGHT = 600;

  static constexpr size_t WINDOW_POSITION_X = 0;
  static constexpr size_t WINDOW_POSITION_Y = 0;
}
