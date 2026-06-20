#pragma once

#include "debug.hpp"
#include "core.hpp"


namespace glu {
    class VAO {
        GLuint id = 0;

        void gen() {
            glGenVertexArrays(1, &id);
        }

    public:
        VAO() {
            gen();
        }
        ~VAO() {
            glDeleteVertexArrays(1, &id);
        }
        VAO(const VAO&) = delete;
        VAO& operator=(const VAO&) = delete;

        VAO(VAO&& other) noexcept {
            this->id = other.id;
            other.id = 0;
        }
        VAO& operator=(VAO&& other) noexcept {
            if (this != &other) {
                glDeleteVertexArrays(1, &id);

                id = other.id;
                other.id = 0;
            }
            return *this;
        }

        void bind() const {
            glBindVertexArray(id);
        }

        void linkAttribute(const int location, const int size, const GLenum type, const int element_stride, const int element_offset) const {
            DEBUG::validateVAOLink(id);

            glVertexAttribPointer(
                location, size, type, GL_FALSE,
                static_cast<GLsizei>(element_stride * getGLTypeSize(type)),
                reinterpret_cast<void*>(element_offset * getGLTypeSize(type)));
            glEnableVertexAttribArray(location);
        }

        void setAttributeDivisor(const int location, const int divisor) const {
            DEBUG::validateVAODivisor(id);
            glVertexAttribDivisor(location, divisor);
        }
    };
}