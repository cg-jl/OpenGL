#pragma once
#include <vector>

namespace glApi {
class VertexBuffer {
   private:
    unsigned int m_rendererID;

   public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    struct Element {
        unsigned int count;
        unsigned int type;
        unsigned int isNormalized;
        static unsigned int getSizeOfType(unsigned int type);
    };

    class Layout {
       private:
        std::vector<Element> m_elements;
        unsigned int m_stride;

       public:
        Layout();

        /**
         * Push an element to the layout.
         * @param count: The number of Ts to push.
         */
        template <typename T>
        void push(unsigned int count);

        const std::vector<Element>& getElements() const;
        unsigned int getStride() const;
    };
};
}  // namespace glApi

inline const std::vector<glApi::VertexBuffer::Element>&
glApi::VertexBuffer::Layout::getElements() const {
    return m_elements;
}
inline unsigned int glApi::VertexBuffer::Layout::getStride() const {
    return m_stride;
}