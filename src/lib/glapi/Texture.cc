#include "Texture.h"

#include <GL/glew.h>
#include <stb_image/stb_image.h>

#include "GLMacros.h"

namespace glApi {
Texture::Texture(const std::string& path)
    : m_filePath(path),
      m_localBuffer(nullptr),
      m_width(0),
      m_height(0),
      m_bpp(0),
      m_rendererID(0) {
    stbi_set_flip_vertically_on_load(1);  // opengl's 0, 0 is down left
    m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

    GLCall(glGenTextures(1, &m_rendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // if called with nullptr it reserves space for the image.
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_localBuffer) {
        stbi_image_free(m_localBuffer);
    }
}

Texture::~Texture() { GLCall(glDeleteTextures(1, &m_rendererID)); }

void Texture::bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
}
void Texture::unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }
}  // namespace glApi