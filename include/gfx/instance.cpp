#include "instance.hpp"

#include <glad/glad.h>
#include "mesh.hpp"

namespace gfx {
    void drawInstances(int instance_count, const Mesh& mesh) {
        glDrawElementsInstanced(
            GL_TRIANGLES,
            mesh.indexCount,
            GL_UNSIGNED_INT,
            nullptr,
            instance_count);
    }

    void drawInstancesBaseVertex(int instance_count, const Mesh& mesh, const int ebo_offset, const int vbo_offset) {
        glDrawElementsInstancedBaseVertex(
            GL_TRIANGLES,
            mesh.indexCount,
            GL_UNSIGNED_INT,
            reinterpret_cast<void*>(ebo_offset * sizeof(unsigned int)),
            instance_count,
            vbo_offset);
    }
}