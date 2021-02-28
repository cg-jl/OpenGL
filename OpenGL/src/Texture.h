#pragma once

#include <string>

class Texture {
private:
    unsigned int m_rendererID;
    std::string m_filePath;
    unsigned char* m_localBuffer;
    int m_width, m_height, m_bpp; // bits per pixel
public:
    Texture(const std::string& path);
    ~Texture();
    

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    int getWidth() const;
    int getHeight() const;
};

inline int Texture::getWidth() const {
    return m_width;
}

inline int Texture::getHeight() const {
    return m_height;
}
