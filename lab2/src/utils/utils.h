#pragma once

#ifdef _MSC_VER
#define CURRENT_FUNCTION __FUNCTION__
#elif defined(__GNUC__)
#define CURRENT_FUNCTION __PRETTY_FUNCTION__
#else
#define CURRENT_FUNCTION __func__
#endif

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>

namespace utils {
  GLuint loadTexture(const std::string& filepath);
  GLuint getTextureId(const std::string& filepath);
}
