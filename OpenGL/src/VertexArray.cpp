#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &m_rendererID));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBuffer::Layout& layout) {
    bind();
    vb.bind();
    const auto& elements = layout.getElements();
    const unsigned int stride = layout.getStride();
    unsigned int offset = 0;
    for (unsigned int i = 0;i < elements.size(); ++i) {
        const auto& el = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, el.count, el.type, el.isNormalized, stride, (const void *)offset));
        offset += el.count * VertexBuffer::Element::getSizeOfType(el.type);
    }
}

void VertexArray::bind() const {
    GLCall(glBindVertexArray(m_rendererID));
}

void VertexArray::unbind() const {
    GLCall(glBindVertexArray(0));
}
