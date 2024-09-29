#pragma once

#include <GL/glut.h>    // Include GLUT for cool drawing utils

#include <glm/vec3.hpp>

#include <constants.h>

namespace states::tor_cylinder_rotate_state::constants {
    static constexpr size_t STAY_SECONDS = 2;
    static constexpr size_t MAX_FRAMES_COUNT = ::constants::FRAMES_PER_SECONDS * STAY_SECONDS;
}
