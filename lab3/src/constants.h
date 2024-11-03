#pragma once

#include <stdio.h> // For size_t
#include <GL/gl.h>

namespace constants {
  static constexpr size_t FRAMES_PER_SECONDS = 60;

  static constexpr GLfloat LOWER_ALPHA = 0.3f;

  // Perspective Projection Settings
  static constexpr GLdouble FOV_Y = 80.0;      // Field of view in the Y direction
  static constexpr GLdouble NEAR_PLANE = 0.01;  // Near clipping plane (positive value)
  static constexpr GLdouble FAR_PLANE = 2000.0; // Far clipping plane

  // Camera Settings
  static constexpr GLdouble CAMERA_POS_X = 0.0;  // Camera position X
  static constexpr GLdouble CAMERA_POS_Y = 0.0; // Move the camera slightly up along the Y-axis
  static constexpr GLdouble CAMERA_POS_Z = 0.0; // Move the camera further back along the Z-axis

  static constexpr GLdouble LOOK_AT_X = 0.0; // Look-at point X
  static constexpr GLdouble LOOK_AT_Y = 0.0; // Look-at point Y
  static constexpr GLdouble LOOK_AT_Z = 10.0; // Look-at point Z

  static constexpr GLdouble UP_X = 0.0; // Up vector X
  static constexpr GLdouble UP_Y = 1.0; // Up vector Y
  static constexpr GLdouble UP_Z = 0.0; // Up vector Z

  // Window Settings
  static constexpr size_t WINDOW_WIDTH = 800;
  static constexpr size_t WINDOW_HEIGHT = 600;
  static constexpr size_t WINDOW_POSITION_X = 0;
  static constexpr size_t WINDOW_POSITION_Y = 0;
} // namespace constants
