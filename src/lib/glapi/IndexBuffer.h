#pragma once

namespace glApi {
class IndexBuffer {
   private:
    unsigned int m_rendererID;
    unsigned int m_count;
    public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    unsigned int getCount() const;
};
}  // namespace glApi

inline unsigned int glApi::IndexBuffer::getCount() const { return m_count; }