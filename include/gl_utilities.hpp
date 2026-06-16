#ifndef GL_UTILITIES_HPP
#define GL_UTILITIES_HPP

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

namespace gl {
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

    struct Shader {
        GLuint id = 0;
        int bufferIndex = -1;

        void compile(const std::string& filePath, const GLenum shaderType) {
            std::string sourceCode = readFile(filePath);
            const char* c_sourceCode = sourceCode.c_str();

            id = glCreateShader(shaderType);
            glShaderSource(id, 1, &c_sourceCode, nullptr);
            glCompileShader(id);
        }
    };

    class ShaderProgram {
        GLuint id = 0;  // im gonna hardcode the shaders for now. also, its not like im gonna need more than one program anyway
                        // but, if i ever do, this architecture would make it easy to refactor
        Shader vertexShader;
        Shader fragmentShader;

    public:
        ~ShaderProgram() {
            glDeleteProgram(id);
        }

        void compileShaders() {
            vertexShader.compile(SOURCE_DIR "shaders/vertexShader.vert", GL_VERTEX_SHADER);
            fragmentShader.compile(SOURCE_DIR "shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
        }

        void build() {
            id = glCreateProgram();
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

    public:
        ~VAO() {
            glDeleteVertexArrays(1, &id);
        }

        void gen() {
            glGenVertexArrays(1, &id);
        }
        void bind() const {
            glBindVertexArray(id);
        }

        void linkAttribute(int location, int size, int item_stride, int item_offset) const {
            if (getCurrentVAO() != id)
                std::cerr << "linked attributes with an unexpected VAO" << " | attempted id: " << id << " | current id:" << getCurrentVAO() << "\n";

            glVertexAttribPointer(
                location, size, GL_FLOAT, GL_FALSE,
                static_cast<GLsizei>(item_stride * sizeof(float)), reinterpret_cast<void*>(item_offset * sizeof(float)));
            glEnableVertexAttribArray(location);
        }

        void setAttributeDivisor(int location, int divisor) const {
            if (getCurrentVAO() != id)
                std::cerr << "set attribute divisor with an unexpected VAO" << " | attempted id: " << id << " | current id:" << getCurrentVAO() << "\n";
            glVertexAttribDivisor(location, divisor);
        }
    };

    class VBO {
        GLuint id = 0;

    public:
        ~VBO() {
            glDeleteBuffers(1, &id);
        }

        void gen() {
            glGenBuffers(1, &id);
        }
        void bind() const {
            glBindBuffer(GL_ARRAY_BUFFER, id);
        }

        void bufferData(const std::vector<float>& data, GLenum usage) const {
            if (getCurrentVBO() != id)
                std::cerr << "buffered data to an unexpected VBO" << " | attempted id: " << id << " | current id:" << getCurrentVBO() << "\n";

            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), usage);
        }
    };
}
#endif