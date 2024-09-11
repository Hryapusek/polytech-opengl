cmake_minimum_required(VERSION 3.22)

project(simple_animation
    VERSION 0.1.0
    DESCRIPTION "This project belongs to the first lab work of polytech"
)

find_package(OpenGL REQUIRED) # Main library
find_package(glfw3 REQUIRED) # Do not really remember
find_library(GLUT glut REQUIRED) # GLUTils library

set(PROJECT_SOURCES
    src/main.cc
    src/states/conus_sphere_stay_state/conus_sphere_stay_state.cc
    src/constants.cc
    src/state_machine/state_machine.cc
    src/states/states.cc
    src/states/conus_sphere_moving_state/conus_sphere_stay_state.cc
    src/states/tor_cylinder_state/tor_cylinder_state.cc
)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} 
  PRIVATE
    ${OPENGL_INCLUDE_DIRS}
    src
    src/states
    src/state_machine
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CMAKE_CXX_STANDARD 23
)


target_sources(${PROJECT_NAME}
  PRIVATE
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE glut OpenGL glfw ${OPENGL_LIBRARIES})
