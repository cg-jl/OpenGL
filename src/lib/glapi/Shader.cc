#include "Shader.h"

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "GLMacros.h"
#include <string_view>
#include <vector>

struct ShaderSource {
  std::string vertexSource, fragmentSource;
};

static ShaderSource parseShader(const std::string &path) {
  std::fstream stream(path);

  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
  ShaderType type = ShaderType::NONE;
  std::string line;
  std::stringstream ss[2];
  while (std::getline(stream, line)) {
    if (line.find("//$shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos) {
        type = ShaderType::FRAGMENT;
      }
      continue;
    }
    if (type == ShaderType::NONE)
      continue;
    ss[(int)type] << line << '\n';
  }
  return {ss[0].str(), ss[1].str()};
}

static unsigned int compileShader(unsigned int type,
                                  const std::string &source) {
  unsigned int id = glCreateShader(type);
  // TODO: Error handling
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, nullptr); // i dont care about the length
  glCompileShader(id);

  // `i` -> integer
  // `v` -> vector
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    // did not compile successfully
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length);
    glGetShaderInfoLog(id, length, &length, message);
    std::cerr << "\x1b[1;31mFailed to compile "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
              << " shader: \x1b[m" << message << std::endl;
    glDeleteShader(id);
    return 0; // shader is not valid
  }

  return id;
}
static unsigned int createShader(const ShaderSource &src) {
  // shader source code -> compiled & linked -> shader ID to bind
  unsigned int programID = glCreateProgram();
  // create two shader objects
  unsigned int fs = compileShader(GL_FRAGMENT_SHADER, src.fragmentSource);
  unsigned int vs = compileShader(GL_VERTEX_SHADER, src.vertexSource);

  // if any of them failed, just don't make a new program.
  if (fs == 0 || vs == 0) {
    glDeleteProgram(programID);
    return 0;
  }

  // Add sources together
  GLCall(glAttachShader(programID, vs));
  GLCall(glAttachShader(programID, fs));

  // Link them
  GLCall(glLinkProgram(programID));

  // Validate them
  GLCall(glValidateProgram(programID));

  // remove intermediate (obj-like) shader
  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  // we should call glDetachShader after linked (no debug mode)
  // #ifndef _DEBUG
  GLCall(glDetachShader(programID, vs));
  GLCall(glDetachShader(programID, fs));
  // #endif
  return programID;
}
static int compileShader(const std::string &fileName) {
  fprintf(stderr, "compiling shader %s\n", fileName.c_str());
  ShaderSource src = parseShader(fileName);
  fprintf(stderr, "source fragment:\n");
  fputs(src.fragmentSource.c_str(), stderr);
  fputs("\nsource vertex:\n", stderr);
  fputs(src.vertexSource.c_str(), stderr);
  fputc('\n', stderr);
  return createShader(src);
}

namespace glApi {
Shader::Shader(const std::string &fileName)
    : m_fileName(fileName), m_rendererID(0) {
  m_rendererID = compileShader(fileName);
}

Shader::~Shader() { GLCall(glDeleteProgram(m_rendererID)); }

void Shader::bind() const { GLCall(glUseProgram(m_rendererID)); }
void Shader::unbind() const { GLCall(glUseProgram(0)); }

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2,
                          float v3) {

  glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniform1i(const std::string &name, int value) {
  glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform1f(std::string const &name, float v) {
  glUniform1f(getUniformLocation(name), v);
}

void Shader::setUniform2f(std::string const &name, float v0, float v1) {
  glUniform2f(getUniformLocation(name), v0, v1);
}

struct uniform_data {
  uintptr_t ptr;
  size_t size;
  unsigned int type;
};

void Shader::reload() {
  fprintf(stderr, "info: reloading shader '%s'\n", m_fileName.c_str());

  // 1. Compile the new shader
  auto const new_program_id = compileShader(m_fileName);
  // If we couldn't recompile, then don't reload.
  if (new_program_id == 0)
    return;
  // now we can remove the other program and replace it.
  GLCall(glDeleteProgram(m_rendererID));
  m_rendererID = new_program_id;

  // 2. Transfer the uniforms to the new shader

  /* int active_uniforms; */
  /* GLCall(glGetProgramiv(m_rendererID, GL_ACTIVE_UNIFORMS, &active_uniforms));
   */

  /* std::cout << "no of active uniforms: " << active_uniforms << '\n'; */

  // std::unordered_map<std::string, uniform_data> backups;

  // size_t max_count = 0;

  // compute the maximum size in bytes we need for the memory.
  // We'll allocate just one block and reuse it.
  /* size_t max_count = 0; */

  /* for (auto const &[k, v] : m_uniformCache) { */
  /* } */

  /* void *const mem = max_count == 0 ? NULL : malloc(max_count); */
  /* if (mem == NULL) */
  /*   return; */

  /* for (auto &[k, v] : m_uniformCache) { */
  /*   auto const old_location = v.location; */
  /*   int new_location; */
  /*   std::cout << "debug: setting '" << k << "'\n"; */
  /*   GLCall(new_location = glGetUniformLocation(new_program_id, k.c_str()));
   */
  /*   v.location = new_location; */
  /*   if (new_location == -1 || old_location == -1) */
  /*     continue; */

  /*   if (v.last_count != 0) */
  /*     switch (v.last_type) { */
  /*     case GL_FLOAT: { */
  /*       size_t const buf_size = sizeof(float) * v.last_count; */
  /*       GLCall(glUseProgram(m_rendererID)); */
  /*       GLCall(glGetnUniformfv(m_rendererID, old_location, buf_size, */
  /*                              (GLfloat *)mem)); */
  /*       GLCall(glUseProgram(new_program_id)); */
  /*       switch (v.last_count) { */
  /*       case 1: */
  /*         GLCall(glUniform1fv(new_location, 1, (GLfloat *)mem)); */
  /*         break; */
  /*       case 2: */
  /*         GLCall(glUniform2fv(new_location, 2, (GLfloat *)mem)); */
  /*         break; */
  /*       case 3: */
  /*         GLCall(glUniform3fv(new_location, 3, (GLfloat *)mem)); */
  /*         break; */
  /*       case 4: */
  /*         GLCall(glUniform4fv(new_location, 4, (GLfloat *)mem)); */
  /*         break; */
  /*       default: */
  /*         break; // we don't have any matrices yet */
  /*       } */
  /*       break; */
  /*     } */
  /*     case GL_INT: { */
  /*       size_t const buf_size = sizeof(int) * v.last_count; */
  /*       GLCall(glUseProgram(m_rendererID)); */
  /*       GLCall(glGetnUniformiv(m_rendererID, old_location, buf_size, */
  /*                              (GLint *)mem)); */
  /*       GLCall(glUseProgram(new_program_id)); */
  /*       switch (v.last_count) { */
  /*       case 1: */
  /*         GLCall(glUniform1iv(new_location, 1, (GLint *)mem)); */
  /*         break; */
  /*       case 2: */
  /*         GLCall(glUniform2iv(new_location, 2, (GLint *)mem)); */
  /*         break; */
  /*       case 3: */
  /*         GLCall(glUniform3iv(new_location, 3, (GLint *)mem)); */
  /*         break; */
  /*       case 4: */
  /*         GLCall(glUniform4iv(new_location, 4, (GLint *)mem)); */
  /*         break; */
  /*       default: */
  /*         break; // we don't have any matrices yet */
  /*       } */
  /*     } break; */
  /*     } */
  /* } */

  std::cout << "info: reloaded shader '" << m_fileName << "'\n";
}

int Shader::getUniformLocation(const std::string &name) {
  const auto found = m_uniformCache.find(name);
  if (found != m_uniformCache.end()) {
    return found->second;
  }
  int loc = glGetUniformLocation(m_rendererID, name.c_str());
  if (loc == -1) {
    std::cout << "warn: uniform '" << name << "' doesn't exist!\n";
  } else {
  }
  return loc;
}

} // namespace glApi
