#pragma once
#include "VertexBuffer.h"

namespace glApi {
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
}  // namespace glApi