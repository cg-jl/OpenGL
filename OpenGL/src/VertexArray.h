#pragma once
#include "VertexBuffer.h"


class VertexArray {
private:
    unsigned int m_rendererID;
public:
    VertexArray();
    ~VertexArray();


    void addBuffer(const VertexBuffer& vb, const VertexBuffer::Layout& layout);

    void bind() const;
    void unbind() const;
};
