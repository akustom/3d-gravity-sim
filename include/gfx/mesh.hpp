#pragma once

#include <glm/glm.hpp>
#include "vertex.hpp"


namespace gfx {
    struct Mesh {
        std::vector<vertex> vertices = {};

        std::vector<glm::uvec3> indices = {};

        void loadModel() {}
    };

    /**turns your mesh into a square*/
    inline void makeSquare(Mesh& mesh) {
        mesh.vertices = {
            {{-0.25f, 0.25f, 0.0f},     {1.0f, 0.0f, 0.0f}},
            {{-0.25f, -0.25f, 0.0f},    {0.0f, 1.0f, 0.0f}},
            {{0.25f, -0.25f, 0.0f},     {0.0f, 0.0f, 1.0f}},
            {{0.25f, 0.25f, 0.0f},      {1.0f, 0.0f, 0.0f}},
        };
        mesh.indices = {
            {0, 1, 2},
            {0, 3, 2}
        };
    }

    inline void makePolygon(const float& size, const int& sides, Mesh& mesh) {
        if (sides < 3) return; // A polygon needs at least 3 sides
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