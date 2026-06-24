#pragma once

#include <vector>
#include <glad/glad.h>
#include "debug.hpp"
#include "gl_utils.hpp"
#include "util.hpp"


namespace glw {
    struct Buffer {
        GLuint id = 0;

        void create() {
            glCreateBuffers(1, &id);
        }

        Buffer() {
            create();
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

                this->id = other.id;
                other.id = 0;
            }
            return *this;
        }

        template <trivially_copyable T>
        void allocateBuffer(std::vector<T>& init_data, const GLenum flag = 0) const {
            glNamedBufferStorage(id, bytesof(init_data), init_data.data(), flag);
        }

        template <trivially_copyable T>
        void pushData(const int byte_offset, std::vector<T>& data) const {
            glNamedBufferSubData(id, byte_offset, bytesof(data), data.data());
        }
        template <trivially_copyable T>
        void pushData(const int byte_offset, T& data) const {
            glNamedBufferSubData(id, byte_offset, bytesof<T>(), getPtr(data));
        }
    };

    struct VBO : Buffer {};

    struct EBO : Buffer {};

    struct UBO : Buffer {
        void bind(const GLint binding_point) const {
            glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, id);
        }

        /**pass vec4 counts (16 byte chunks) into vec4_offset*/
        void allocateBuffer(const GLuint byte_offset) const {
            glNamedBufferStorage(id, static_cast<GLintptr>(byte_offset), nullptr, GL_DYNAMIC_STORAGE_BIT);
        }

        /**pass vec4 counts (16 byte chunks) into vec4_offset*/
        template <trivially_copyable D>
        void pushUniform(const GLuint byte_offset, const D& data) const {
            this->pushData(static_cast<GLintptr>(byte_offset), data);
        }
    };
}
