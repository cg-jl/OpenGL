#pragma once

#include <string>
#include <unordered_map>

class Shader {
private:
    // debugging purposes
    std::string m_fileName;
    unsigned int m_rendererID;

    // caching for uniforms
    std::unordered_map<std::string, int> m_uniformCache;
public:
    Shader(const std::string& fileName);
    ~Shader();

    void bind() const;
    void unbind() const;

    // set uniforms
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniform1i(const std::string& name, int value);
private:
    bool compileShader();
    int getUniformLocation(const std::string& name);
};
