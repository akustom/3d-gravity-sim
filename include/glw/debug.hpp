#pragma once

#include <iostream>
#include <glad/glad.h>


#ifndef NDEBUG
namespace DEBUG {
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

    inline GLuint getCurrentUBO() {
        GLint current = 0;
        glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &current);
        return current;
    }

    inline void validateVAOLink(GLuint id) {
        if (getCurrentVAO() != id)
            std::cerr << "linked attributes with an unexpected VAO" << " | attempted id: " << id << " | current id:" << getCurrentVAO() << "\n";
    }
    inline void validateVAODivisor(GLuint id) {
        if (getCurrentVAO() != id)
            std::cerr << "set attribute divisor with an unexpected VAO" << " | attempted id: " << id << " | current id:" << getCurrentVAO() << "\n";
    }

    inline void validateVBOBuffer(GLuint id) {
        if (getCurrentVBO() != id)
            std::cerr << "buffered data to an unexpected VBO" << " | attempted id: " << id << " | current id:" << getCurrentVBO() << "\n";
    }

    inline void validateEBOBuffer(GLuint id) {
        if (getCurrentEBO() != id)
            std::cerr << "buffered data to an unexpected EBO" << " | attempted id: " << id << " | current id:" << getCurrentEBO() << "\n";
    }

    inline void validateUBOAllocate(GLuint id) {
        if (getCurrentUBO() != id)
            std::cerr << "buffered data to an unexpected UBO" << " | attempted id: " << id << " | current id:" << getCurrentUBO() << "\n";
    }
    inline void validateUBOPush(GLuint id) {
        if (getCurrentUBO() != id)
            std::cerr << "pushed uniform data to an unexpected UBO" << " | attempted id: " << id << " | current id:" << getCurrentUBO() << "\n";
    }
}

#else
class DEBUG {
    public:
    static void validateVAOLink(GLuint id) {}
    static void validateVAODivisor(GLuint id) {}
    static void validateVBOBuffer(GLuint id) {}
    static void validateEBOBuffer(GLuint id) {}
    static void validateUBOAllocate(GLuint id) {}
    static void validateUBOPush(GLuint id) {}
};
#endif