#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

namespace glApi {
class Renderer {
   public:
    void draw(const VertexArray& va, const IndexBuffer& ib,
              const Shader& shader) const;  // shader? TODO: use materials.
    void clear() const;
};
}  // namespace glApi