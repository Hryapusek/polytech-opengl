#pragma once

#include <GL/glut.h>    // Include GLUT for cool drawing utils
#include <GLFW/glfw3.h> // Include GLFW for window and context management

#include <glm/vec3.hpp>

#include <constants.h>

namespace states::conus_sphere_stay_state::constants {

    static constexpr size_t STAY_SECONDS = 3;
    static constexpr size_t MAX_FRAMES_COUNT = ::constants::FRAMES_PER_SECONDS * STAY_SECONDS;

    static constexpr GLdouble cone_base = 20.0;
    static constexpr GLdouble cone_height = 200.0;
    static constexpr GLint cone_slices = 500;
    static constexpr GLint cone_stacks = 500;

    static constexpr GLint sphere_radius = 20;
    static constexpr GLint sphere_slices = 100;
    static constexpr GLint sphere_stacks = 100;

    static constexpr glm::dvec3 cone_start_pos = glm::dvec3(-20, 0, -19);
    static constexpr glm::dvec3 sphere_start_pos = glm::dvec3(20, 0, 0.);

    static constexpr glm::dvec3 cone_end_pos = glm::dvec3(0, 0, -19);
    static constexpr glm::dvec3 sphere_end_pos = glm::dvec3(0, 0, 0.);

    static constexpr GLdouble sphere_start_pos_x = -20.;
    static constexpr GLdouble sphere_start_pos_y = 0.;

}
