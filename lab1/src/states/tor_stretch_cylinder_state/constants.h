#pragma once

#include <GL/glut.h>    // Include GLUT for cool drawing utils

#include <glm/vec3.hpp>

#include <constants.h>

namespace states::tor_stretch_cylinder_state::constants {
    static constexpr size_t STAY_SECONDS = 1;
    static constexpr size_t MAX_FRAMES_COUNT = ::constants::FRAMES_PER_SECONDS * STAY_SECONDS;

    static constexpr glm::dvec3 torus_result_scale = glm::dvec3(1, 2, 1);
}
