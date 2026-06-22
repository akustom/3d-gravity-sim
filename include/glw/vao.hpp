#pragma once

#include <vector>
#include <glad/glad.h>
#include "debug.hpp"
#include "gl_utils.hpp"
#include "glw/buffer.hpp"


namespace glw {
    class VAO {
        GLuint id = 0;

        void gen() {
            glCreateVertexArrays(1, &id);
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

        /**use offsetof(struct, member) to calculate byte_offset*/
        void formatAttribute(const int attrib_loc, const int binding_loc, const int size, const GLenum type, const int byte_offset) const {
            glEnableVertexArrayAttrib(id, attrib_loc);
            glVertexArrayAttribFormat(id, attrib_loc, size, type, GL_FALSE, byte_offset);
            glVertexArrayAttribBinding(id, attrib_loc, binding_loc);
        }

        void setAttributeDivisor(const int binding_loc, const int divisor) const {
            glVertexArrayBindingDivisor(id, binding_loc, divisor);
        }

        /**use offsetof(struct, member) to calculate byte_offset,*/
        template <trivially_copyable T>
        void attachBuffer(const VBO& vbo, const int binding_loc, const int byte_offset, const std::vector<T>&) const {
            glVertexArrayVertexBuffer(id, binding_loc, vbo.id, byte_offset, sizeof(T));
        }
        void attachBuffer(const EBO& ebo) const {
            glVertexArrayElementBuffer(id, ebo.id);
        }
    };
}