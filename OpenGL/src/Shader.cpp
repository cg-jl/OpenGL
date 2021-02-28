#include "Shader.h"
#include "Renderer.h"
#include "GLMacros.h"
#include <GL/glew.h>
#include <sstream>
#include <fstream>
#include <iostream>

struct ShaderSource {
    std::string vertexSource, fragmentSource;
};


static ShaderSource parseShader(const std::string& path) {
    std::fstream stream(path);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    ShaderType type = ShaderType::NONE;
    std::string line;
    std::stringstream ss[2];
    size_t commentPos;
    while (std::getline(stream, line)) {
        if (line.find("//$shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
            continue;
        }
        if (type == ShaderType::NONE) continue;
        ss[(int)type] << line << '\n';
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    // TODO: Error handling
    const char* src = source.c_str();
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
        char* message = (char*)alloca(length);
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "\x1b[1;31mFailed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: \x1b[m" << message << std::endl;
        glDeleteShader(id);
        return 0; // shader is not valid
    }

    return id;
}
static unsigned int createShader(const ShaderSource& src) {
    // shader source code -> compiled & linked -> shader ID to bind
    unsigned int programID = glCreateProgram();
    // create two shader objects
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, src.fragmentSource);
    unsigned int vs = compileShader(GL_VERTEX_SHADER, src.vertexSource);

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
#ifndef _DEBUG
    GLCall(glDetachShader(programID, vs));
    GLCall(glDetachShader(programID, fs));
#endif
    return programID;
}



Shader::Shader(const std::string& fileName) : m_fileName(fileName), m_rendererID(0) {
    compileShader();
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_rendererID));
}

bool Shader::compileShader() {
    ShaderSource src = parseShader(m_fileName);
    m_rendererID = createShader(src);
    return true;
}

void Shader::bind() const {
    GLCall(glUseProgram(m_rendererID));
}
void Shader::unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {

    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniform1i(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

int Shader::getUniformLocation(const std::string& name) {
    const auto found = m_uniformCache.find(name);
    if (found != m_uniformCache.end()) {
        return found->second;
    }
    int loc = glGetUniformLocation(m_rendererID, name.c_str());
    if (loc == -1) {
        std::cout << "Warning:: uniform '" << name << "' doesn't exist!\n";
    }
    else {
        m_uniformCache.insert({ name, loc });
    }
    return loc;
}
