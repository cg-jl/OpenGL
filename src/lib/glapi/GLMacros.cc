#include "GLMacros.h"

#include <GL/glew.h>
#include <iostream>

void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}

bool GLCheckError(const char *function, const char *file, int line) {
  bool got = true;
  static std::string error;
  while (GLenum errorCode = glGetError()) {
    switch (errorCode) {
    case GL_INVALID_ENUM:
      error = "Invalid Enum";
      break;
    case GL_INVALID_VALUE:
      error = "Invalid Value";
      break;
    case GL_INVALID_OPERATION:
      error = "Invalid operation";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      error = "Invalid framebuffer operation";
      break;
    case GL_OUT_OF_MEMORY:
      error = "Out of Memory";
      break;
    case GL_STACK_UNDERFLOW:
      error = "Stack Underflow";
      break;
    case GL_STACK_OVERFLOW:
      error = "Stack Overflow";
      break;
    default:
      error = "Unkown error";
      break;
    }
    std::cerr << "\x1b[1;31m[OPENGL]\x1b[38;5;227m <" << errorCode << ">\x1b[m "
              << error << " [\x1b[38;5;31m" << file << " \x1b[38;5;155m"
              << function << "\x1b[m \x1b[38;5;219m" << line << "\x1b[m]"
              << std::endl;
    got = false;
  }
  return got;
}
