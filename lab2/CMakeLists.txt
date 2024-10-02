cmake_minimum_required(VERSION 3.22)

project(object_with_textures
    VERSION 0.1.0
    DESCRIPTION "This project belongs to the second lab work of polytech"
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets OpenGL)

find_package(OpenGL REQUIRED) # Main library
find_library(GLUT glut REQUIRED) # GLUTils library
find_package(GLEW REQUIRED)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(PROJECT_SOURCES
    src/main.cc
    src/constants.cc

    src/state_machine/state_machine.cc

    src/states/states.cc
    src/states/conus_sphere_stay_state/conus_sphere_stay_state.cc
    src/states/conus_sphere_moving_state/conus_sphere_stay_state.cc
    src/states/tor_cylinder_state/tor_cylinder_state.cc
    src/states/tor_cylinder_rotate_state/tor_cylinder_rotate_state.cc
    src/states/tor_stretch_cylinder_state/tor_stretch_cylinder_state.cc

    src/glwidget/glwidget.cc

    src/main_window/main_window.cc

    src/params_window/params_window.cc

    src/custom_widgets/fancy_slider.cc

    src/utils/utils.cc
)

add_executable(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)

target_include_directories(${PROJECT_NAME} 
  PRIVATE
    ${OPENGL_INCLUDE_DIRS}
    src
    src/states
    src/state_machine
    external
    ${GLEW_INCLUDE_DIRS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CMAKE_CXX_STANDARD 23
)

target_sources(${PROJECT_NAME}
  PRIVATE
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::Core
    glut
    OpenGL
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
)
