#pragma once
#include <vector>
#include <GL/glew.h>
#include "Renderer.h"


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
        static unsigned int getSizeOfType(unsigned int type) {
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
    };



    class Layout {
        std::vector<Element> m_elements;
        unsigned int m_stride;
    public:
        Layout() 
            : m_stride(0) {}

        template<typename T>
        void push(unsigned int count) {
            static_assert(false, "Not implemented for any type");
        }

        template<>
        void push<float>(unsigned int count) {
            m_elements.push_back({ count, GL_FLOAT, GL_FALSE });
            m_stride += Element::getSizeOfType(GL_FLOAT) * count;
        }


        template<>
        void push<unsigned int>(unsigned int count) {
            m_elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
            m_stride += Element::getSizeOfType(GL_UNSIGNED_INT) * count;
        }


        template<>
        void push<unsigned char>(unsigned int count) {
            m_elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
            m_stride += Element::getSizeOfType(GL_UNSIGNED_BYTE) * count;
        }

        inline const std::vector<Element>& getElements() const noexcept {
            return m_elements;
        }

        inline unsigned int getStride() const noexcept {
            return m_stride;
        }
    };

};

