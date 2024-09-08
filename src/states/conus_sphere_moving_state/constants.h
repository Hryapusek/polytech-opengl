#pragma once

#include <GL/glut.h>    // Include GLUT for cool drawing utils
#include <GLFW/glfw3.h> // Include GLFW for window and context management

#include <glm/vec3.hpp>

#include <constants.h>

namespace states::conus_sphere_moving_state::constants {

    static constexpr size_t ANIMATION_SECONDS = 3;
    static constexpr size_t MAX_ANIMATION_FRAMES = ANIMATION_SECONDS * ::constants::FRAMES_PER_SECONDS;

    static constexpr glm::dvec3 cone_end_pos = glm::dvec3(0, 0, -19);
    static constexpr glm::dvec3 sphere_end_pos = glm::dvec3(0, 0, -19.0);
}
