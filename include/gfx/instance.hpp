#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "mesh.hpp"


namespace gfx {
    struct Instance {
        glm::dvec4 position;

        void drawInstance(const Mesh& mesh) const {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(3 * mesh.indices.size()), GL_UNSIGNED_INT, nullptr);
        }
    };


    struct Instances {
        std::vector<glm::dvec4> instancePos = {
            {0.0, 0.0, 0.0, 1.0},
        };

        void drawInstances(const Mesh& mesh) const {
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(3 * mesh.indices.size()), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(instancePos.size()));
        }
    };
}