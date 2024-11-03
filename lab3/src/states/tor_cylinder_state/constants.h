#pragma once

#include <GL/glut.h>    // Include GLUT for cool drawing utils

#include <glm/vec3.hpp>

#include <constants.h>

namespace states::tor_cylinder_state::constants {

    static constexpr size_t STAY_SECONDS = 1;
    static constexpr size_t MAX_FRAMES_COUNT = ::constants::FRAMES_PER_SECONDS * STAY_SECONDS;

    static constexpr GLdouble torus_inner_radius = 5;
    static constexpr GLdouble torus_outer_radius = 10;
    static constexpr GLint torus_sides = 100;
    static constexpr GLint torus_rings = 100;

    static constexpr GLint cylinder_base = 10;
    static constexpr GLint cylinder_top = 10;
    static constexpr GLint cylinder_height = 40;
    static constexpr GLint cylinder_slices = 50;
    static constexpr GLint cylinder_stacks = 50;

    static constexpr glm::dvec3 torus_start_pos = glm::dvec3(-10, 0, 5);
    static constexpr glm::dvec3 cylinder_start_pos = glm::dvec3(-10, 10, -20);

    static constexpr GLfloat mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };   // Low intensity ambient (grey)
    static constexpr GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 0.5f };    // Bright green diffuse (matte green)
    static constexpr GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };   // No specular highlights for matte
    static constexpr GLfloat mat_shininess[] = { 0.0f };                     // Shininess (set to 0 for matte finish)

    static constexpr const char *cone_texture_path = "textures/cone_texture.png";
}
