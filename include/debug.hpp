#pragma once
#include "glad/glad.h"

#ifndef NDEBUG
class DEBUG {
private:
    static GLuint getCurrentVAO() {
        GLint current = 0;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current);
        return current;
    }

    static GLuint getCurrentVBO() {
        GLint current = 0;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current);
        return current;
    }

    static GLuint getCurrentEBO() {
        GLint current = 0;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &current);
        return current;
    }

    static GLuint getCurrentUBO() {
        GLint current = 0;
        glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &current);
        return current;
    }

public:
    static void validateVAOLink(GLuint id) {
        if (getCurrentVAO() != id)
            std::cerr << "linked attributes with an unexpected VAO" << " | attempted id: " << id << " | current id:" << getCurrentVAO() << "\n";
    }
    static void validateVAODivisor(GLuint id) {
        if (getCurrentVAO() != id)
            std::cerr << "set attribute divisor with an unexpected VAO" << " | attempted id: " << id << " | current id:" << getCurrentVAO() << "\n";
    }

    static void validateVBOBuffer(GLuint id) {
        if (getCurrentVBO() != id)
            std::cerr << "buffered data to an unexpected VBO" << " | attempted id: " << id << " | current id:" << getCurrentVBO() << "\n";
    }

    static void validateEBOBuffer(GLuint id) {
        if (getCurrentEBO() != id)
            std::cerr << "buffered data to an unexpected EBO" << " | attempted id: " << id << " | current id:" << getCurrentEBO() << "\n";
    }

    static void validateUBOAllocate(GLuint id) {
        if (getCurrentUBO() != id)
            std::cerr << "buffered data to an unexpected UBO" << " | attempted id: " << id << " | current id:" << getCurrentUBO() << "\n";
    }
    static void validateUBOPush(GLuint id) {
        if (getCurrentUBO() != id)
            std::cerr << "pushed uniform data to an unexpected UBO" << " | attempted id: " << id << " | current id:" << getCurrentUBO() << "\n";
    }
};

#else
class DEBUG {
    public:
    static void validateVAOLink(GLuint id) {}
    static void validateVAODivisor(GLuint id) {}
    static void validateVBOBuffer(GLuint id) {}
    static void validateEBOBuffer(GLuint id) {}
};
#endif