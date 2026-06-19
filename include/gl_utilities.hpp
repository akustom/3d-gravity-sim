#pragma once

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>

inline std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "failed to open file: " << filePath << "\n";
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

template <typename T>
    concept trivially_copyable = std::is_trivially_copyable_v<T>;

namespace glu {
    int getGLTypeSize(auto type) {
        switch (type) {
            case GL_FLOAT:  return sizeof(float);
            case GL_INT:    return sizeof(int);
            case GL_DOUBLE: return sizeof(double);
            default:
                std::cerr << "unexpected GL type: " << type << "\n";
                return 0;
        }
    }

    inline GLuint getCurrentVAO() {
        GLint current = 0;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current);
        return current;
    }

    inline GLuint getCurrentVBO() {
        GLint current = 0;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current);
        return current;
    }

    inline GLuint getCurrentEBO() {
        GLint current = 0;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &current);
        return current;
    }

    struct Shader {
        GLuint id = 0;
        int bufferIndex = -1;

        Shader(const std::string& filePath, const GLenum shaderType) {
            compile(filePath, shaderType);
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

        void compile(const std::string& filePath, const GLenum shaderType) {
            std::string sourceCode = readFile(filePath);
            const char* c_sourceCode = sourceCode.c_str();

            id = glCreateShader(shaderType);
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


        void build(const Shader& vertexShader, const Shader& fragmentShader) const {
            glAttachShader(id, vertexShader.id);
            glAttachShader(id, fragmentShader.id);
            glLinkProgram(id);

            glDeleteShader(vertexShader.id);
            glDeleteShader(fragmentShader.id);
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

        void linkAttribute(int location, int size, GLenum type, int element_stride, int element_offset) const {
            if (getCurrentVAO() != id)
                std::cerr << "linked attributes with an unexpected VAO" << " | attempted id: " << id << " | current id:" << getCurrentVAO() << "\n";

            glVertexAttribPointer(
                location, size, type, GL_FALSE,
                static_cast<GLsizei>(element_stride * getGLTypeSize(type)), reinterpret_cast<void*>(element_offset * getGLTypeSize(type)));
            glEnableVertexAttribArray(location);
        }

        void setAttributeDivisor(int location, int divisor) const {
            if (getCurrentVAO() != id)
                std::cerr << "set attribute divisor with an unexpected VAO" << " | attempted id: " << id << " | current id:" << getCurrentVAO() << "\n";
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

        void bind(GLenum bufferType) const {
            glBindBuffer(bufferType, id);
        }

        template <trivially_copyable Type>
        static void bufferData(GLenum bufferType, const std::vector<Type>& data, GLenum usage) {
            glBufferData(bufferType, static_cast<GLsizeiptr>(data.size() * sizeof(Type)), data.data(), usage);
        }
    };

    struct VBO : Buffer {
        void bind() const {
            Buffer::bind(GL_ARRAY_BUFFER);
        }
        void bufferData(const auto& data, GLenum usage) {
            GLuint currentBuffer = getCurrentVBO();
            if (currentBuffer != id)
                std::cerr << "buffered data to an unexpected VBO" << " | attempted id: " << id << " | current id:" << currentBuffer << "\n";
            Buffer::bufferData(GL_ARRAY_BUFFER, data, usage);
        }
    };

    struct EBO : Buffer {
        void bind() const {
            Buffer::bind(GL_ELEMENT_ARRAY_BUFFER);
        }
        void bufferData(const auto& data, GLenum usage) {
            GLuint currentBuffer = getCurrentEBO();
            if (currentBuffer != id)
                std::cerr << "buffered data to an unexpected EBO" << " | attempted id: " << id << " | current id:" << currentBuffer << "\n";
            Buffer::bufferData(GL_ELEMENT_ARRAY_BUFFER, data, usage);
        }
    };
}