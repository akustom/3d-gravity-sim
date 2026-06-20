#pragma once

#include <vector>
#include "debug.hpp"
#include "core.hpp"


namespace glu {
    struct Buffer {
    protected:
        GLuint id = 0;

        void gen() {
            glGenBuffers(1, &id);
        }

    public:
        Buffer() {
            gen();
        }
        ~Buffer() {
            glDeleteBuffers(1, &id);
        }
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        Buffer(Buffer&& other) noexcept {
            this->id = other.id;
            other.id = 0;
        }
        Buffer& operator=(Buffer&& other) noexcept {
            if (this != &other) {
                glDeleteBuffers(1, &id);

                id = other.id;
                other.id = 0;
            }
            return *this;
        }

        void bind(GLenum buffer_type) const {
            glBindBuffer(buffer_type, id);
        }

        template <trivially_copyable T>
        static void bufferData(GLenum buffer_type, const std::vector<T>& data, GLenum usage) {
            glBufferData(buffer_type, static_cast<GLsizeiptr>(data.size() * sizeof(T)), data.data(), usage);
        }
    };

    struct VBO : Buffer {
        void bind() const {
            Buffer::bind(GL_ARRAY_BUFFER);
        }
        void bufferData(const auto& data, GLenum usage) {
            DEBUG::validateVBOBuffer(id);
            Buffer::bufferData(GL_ARRAY_BUFFER, data, usage);
        }
    };

    struct EBO : Buffer {
        void bind() const {
            Buffer::bind(GL_ELEMENT_ARRAY_BUFFER);
        }
        void bufferData(const auto& data, GLenum usage) {
            DEBUG::validateEBOBuffer(id);
            Buffer::bufferData(GL_ELEMENT_ARRAY_BUFFER, data, usage);
        }
    };

    struct UBO : Buffer {
        explicit UBO() {
            gen();
        }

        void bind(const GLint binding_point) const {
            glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, id);
        }

        /**pass vec4 counts (16 byte chunks) into vec4_offset*/
        void allocateBuffer(const GLintptr vec4_count) const {
            DEBUG::validateUBOAllocate(id);
            glBufferData(GL_UNIFORM_BUFFER, vec4_count * 16, nullptr, GL_DYNAMIC_DRAW);
        }

        /**pass vec4 counts (16 byte chunks) into vec4_offset*/
        template <trivially_copyable D>
        void pushUniform(const GLintptr vec4_offset, const D& data) const {
            DEBUG::validateUBOPush(id);
            glBufferSubData(GL_UNIFORM_BUFFER, vec4_offset * 16, sizeof(D), getPtr(data));
        }
    };
}
