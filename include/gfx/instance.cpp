#include "instance.hpp"

#include <glad/glad.h>
#include "mesh.hpp"

namespace gfx {
    void drawInstances(int instance_count, const Mesh& mesh) {
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(3 * mesh.indices.size()), GL_UNSIGNED_INT, nullptr, instance_count);
    }
}