#pragma once

#include "core.hpp"


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

}