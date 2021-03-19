#include "VertexBuffer.h"

#include <GL/glew.h>

#include "GLMacros.h"

namespace glApi {
VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    GLCall(glGenBuffers(1, &m_rendererID));

    // select buffer (bind)
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
    // put data into the buffer
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() { GLCall(glDeleteBuffers(1, &m_rendererID)); }

void VertexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void VertexBuffer::unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }


VertexBuffer::Layout::Layout() : m_stride(0) {}

template <>
void VertexBuffer::Layout::push<float>(unsigned int count) {
    m_elements.push_back({count, GL_FLOAT, GL_FALSE});
    m_stride += VertexBuffer::Element::getSizeOfType(GL_FLOAT) * count;
}
template <>
void VertexBuffer::Layout::push<unsigned int>(unsigned int count) {
    m_elements.push_back({count, GL_UNSIGNED_INT, GL_FALSE});
    m_stride += VertexBuffer::Element::getSizeOfType(GL_UNSIGNED_INT) * count;
}

template <>
void VertexBuffer::Layout::push<unsigned char>(unsigned int count) {
    m_elements.push_back({count, GL_UNSIGNED_BYTE, GL_TRUE});
    m_stride += VertexBuffer::Element::getSizeOfType(GL_UNSIGNED_BYTE) * count;
}

unsigned int VertexBuffer::Element::getSizeOfType(unsigned int type) {
    switch (type) {
        case GL_FLOAT:
        case GL_UNSIGNED_INT:
            return 4;

        case GL_UNSIGNED_BYTE:
            return 1;
    }
    ASSERT(false);
    return 0;
}
}  // namespace glApi