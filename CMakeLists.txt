cmake_minimum_required(VERSION 3.22)

project(simple_animation
    VERSION 0.1.0
    DESCRIPTION "This project belongs to the first lab work of polytech"
)

find_package(OpenGL REQUIRED) # Main library
find_package(glfw3 REQUIRED) # Do not really remember
find_library(GLEW GLEW REQUIRED) # Extension library. Not even used here, but linked just in case
find_library(GLUT glut REQUIRED) # GLUTils library

set(PROJECT_SOURCES
    src/main.cc
    src/states/conus_sphere_stay_state/conus_sphere_stay_state.cc
    src/constants.cc
    src/state_machine/state_machine.cpp
    src/states/states.cpp
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

target_link_libraries(${PROJECT_NAME} PRIVATE glut OpenGL glfw GLEW ${OPENGL_LIBRARIES})
