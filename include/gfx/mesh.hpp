#pragma once

#include <glm/glm.hpp>
#include "vertex.hpp"


namespace gfx {
    struct Mesh {
        std::vector<vertex> vertices = {};

        std::vector<glm::uvec3> indices = {};

        void loadModel() {}
    };

    /**turns your Mesh into a polygon of n-sides*/
    inline void makePolygon(Mesh& mesh, const float& size, const int& sides) {
        if (sides < 3) return;
        std::vector<vertex> vertices;
        std::vector<glm::uvec3> indices;

        vertices.reserve(sides + 1);
        vertices.push_back({{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});

        float angleIncrement = glm::radians(360.0f / static_cast<float>(sides));

        for (int i = 0; i < sides; ++i) {
            float currentAngle = angleIncrement * static_cast<float>(i);
            glm::vec3 pos = glm::vec3(
                size * std::cos(currentAngle),
                size * std::sin(currentAngle),
                0.0f
            );
            vertices.push_back({pos, {1.0f, 0.0f, 0.0f}});
        }

        for (int i = 1; i <= sides; ++i) {
            GLuint center = 0;
            GLuint current = i;
            GLuint next = i == sides ? 1 : i + 1;

            indices.emplace_back(center, current, next);
        }

        mesh.vertices = vertices;
        mesh.indices = indices;
    }
}