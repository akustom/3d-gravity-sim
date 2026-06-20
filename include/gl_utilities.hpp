#pragma once

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <debug.hpp>

inline std::string readFile(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "failed to open file: " << file_path << "\n";
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

template <typename T>
    concept trivially_copyable = std::is_trivially_copyable_v<T>;

template<typename T>
constexpr auto getPtr(const T& data) {
    if constexpr (std::is_same_v<T,glm::mat4>)
        return glm::value_ptr(data);
    else
        std::cerr << "uniform type is currently unsupported\n";
};

constexpr int getGLTypeSize(const GLenum type) {
    switch (type) {
        case GL_FLOAT:  return sizeof(float);
        case GL_INT:    return sizeof(int);
        case GL_DOUBLE: return sizeof(double);
        default:
            throw std::invalid_argument("unexpected gl type");
    }
}

namespace glu {
    struct Shader {
        GLuint id = 0;
        int bufferIndex = -1;

        Shader(const std::string& file_path, const GLenum shader_type) {
            compile(file_path, shader_type);
        }
        ~Shader() {
            glDeleteShader(id);
        }
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        Shader(Shader&& other) noexcept {
            id = other.id;
            other.id = 0;
        }
        Shader& operator=(Shader&& other) noexcept {
            if (this != &other) {
                glDeleteShader(id);

                id = other.id;
                other.id = 0;
            }
            return *this;
        }

        void compile(const std::string& file_path, const GLenum shader_type) {
            std::string sourceCode = readFile(file_path);
            const char* c_sourceCode = sourceCode.c_str();

            id = glCreateShader(shader_type);
            glShaderSource(id, 1, &c_sourceCode, nullptr);
            glCompileShader(id);
        }
    };

    class ShaderProgram {
    public:
        GLuint id = 0;

        ShaderProgram() {
            id = glCreateProgram();
        }
        ~ShaderProgram() {
            glDeleteProgram(id);
        }
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        ShaderProgram(ShaderProgram&& other) noexcept {
            id = other.id;
            other.id = 0;
        }
        ShaderProgram& operator=(ShaderProgram&& other) noexcept {
            if (this != &other) {
                glDeleteProgram(id);

                id = other.id;
                other.id = 0;
            }
            return *this;
        }

        void build(const Shader& vertex_shader, const Shader& fragment_shader) const {
            glAttachShader(id, vertex_shader.id);
            glAttachShader(id, fragment_shader.id);
            glLinkProgram(id);

            glDetachShader(id, vertex_shader.id);
            glDetachShader(id, fragment_shader.id);
        }

        void use() const {
            glUseProgram(id);
        }
    };

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
        explicit UBO(const GLint binding) {
            gen();
            glBindBufferBase(GL_UNIFORM_BUFFER, binding, id);
        }

        void bind() const {
            Buffer::bind(GL_UNIFORM_BUFFER);
        }

        /**pass vec4 counts (16 byte chunks) into vec4_offset*/
        static void allocateBuffer(const GLintptr vec4_count) {
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