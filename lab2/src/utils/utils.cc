#include <utils/utils.h>

#include <iostream>
#include <map>
#include <memory>
#include <mutex>

#include <image_loader/stb_image.h>

namespace utils {
  struct CachedTexture
  {
    GLuint id;
    int width;
    int height;
    int nrChannels;
    GLenum format;
    std::unique_ptr<unsigned char[]> data;
  };

  GLuint loadTexture(const std::string& filepath)
  {
    static std::map<std::string, CachedTexture> textureMap;
    static std::once_flag flag;
    auto found_texture = textureMap.find(filepath);
    if (found_texture == textureMap.end()) {
      CachedTexture texture = {};
      glGenTextures(1, &texture.id);
      // Flip the image vertically during loading
      stbi_set_flip_vertically_on_load(true);
      unsigned char* data =
        stbi_load(filepath.c_str(), &texture.width, &texture.height, &texture.nrChannels, 0);
      if (!data) {
        std::cerr << "Failed to load texture at path: " << filepath << std::endl;
        return 0;
      }

      texture.data = std::unique_ptr<unsigned char[]>(data);

      if (texture.nrChannels == 1)
        texture.format = GL_RED;
      else if (texture.nrChannels == 3)
        texture.format = GL_RGB;
      else if (texture.nrChannels == 4)
        texture.format = GL_RGBA;
      else {
        throw std::runtime_error("Unsupported number of channels: " +
                                 std::to_string(texture.nrChannels));
      }

      textureMap[filepath] = std::move(texture);
    }

    auto& texture = textureMap[filepath];

    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the texture data
    gluBuild2DMipmaps(GL_TEXTURE_2D, texture.format, texture.width, texture.height, texture.format,
                      GL_UNSIGNED_BYTE, texture.data.get());

    return texture.id;
  }

  GLuint getTextureId(const std::string& filepath)
  {
    static std::map<std::string, GLuint> textureMap;

    if (textureMap.find(filepath) == textureMap.end()) {
      GLuint textureID = loadTexture(filepath);
      textureMap[filepath] = textureID;
      return textureID;
    } else {
      return textureMap[filepath];
    }
  }
} // namespace utils