#pragma once

#include <vector>
#include <glad/glad.h>
#include "mesh.hpp"


namespace gfx {
    void drawInstances(const auto& data, const Mesh& mesh) {
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(3 * mesh.indices.size()), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(data.size()));
    }
}