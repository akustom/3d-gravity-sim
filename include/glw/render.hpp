#pragma once

#include <glad/glad.h>


namespace glw {
    inline void drawInstances(int instance_count, const int indices_count) {
        glDrawElementsInstanced(
        GL_TRIANGLES,
        indices_count,
        GL_UNSIGNED_INT,
        nullptr,
        instance_count);
    }

    inline void drawInstancesBaseVertex(int instance_count, const int indices_count, const int ebo_offset, const int vbo_offset) {
        glDrawElementsInstancedBaseVertex(
        GL_TRIANGLES,
        indices_count,
        GL_UNSIGNED_INT,
        reinterpret_cast<void*>(ebo_offset * sizeof(unsigned int)),
        instance_count,
        vbo_offset);
    }
}

