#pragma once

#include <vector>
#include <cmath>
#include <glad/glad.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <glm/glm.hpp>
#include "vertex.hpp"


namespace gfx {
    struct Mesh {
        std::vector<vertex> vertices    = {};

        std::vector<glm::uvec3> indices = {};

        void loadModel();
    };

    /**turns your Mesh into a regular polygon of n-sides*/
    inline void makePolygon(Mesh& mesh, const float radius, const int sides) {
        if (sides < 3) return;

        std::vector<vertex> vertices;
        std::vector<glm::uvec3> indices;

        vertices.reserve(sides + 1);
        vertices.push_back({{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});

        float angleIncrement = glm::radians(360.0f / static_cast<float>(sides));

        for (int i = 0; i < sides; ++i) {
            float currentAngle = angleIncrement * static_cast<float>(i);
            glm::vec3 pos = glm::vec3(
                radius * std::cos(currentAngle),
                radius * std::sin(currentAngle),
                0.0f
            );
            vertices.push_back({pos, {1.0f, 0.0f, 0.0f}});
        }

        for (int i = 1; i <= sides; ++i) {
            GLuint center   = 0;
            GLuint current  = i;
            GLuint next     = i == sides ? 1 : i + 1;

            indices.emplace_back(center, current, next);
        }

        mesh.vertices   = vertices;
        mesh.indices    = indices;
    }

    inline std::vector<vertex> getSphereVertices(const float radius, const int sides) {
        std::vector<vertex> vertices;

        float latitudeIncrement = glm::radians(180.0f / static_cast<float>(sides));
        float longitudeIncrement = glm::radians(360.0f / static_cast<float>(sides));

        for (int i = 0; i <= sides; ++i) {
            float currentLatitudeAngle = -glm::half_pi<float>() + latitudeIncrement * static_cast<float>(i);
            float currentY = std::sin(currentLatitudeAngle) * radius;

            for (int j = 0; j < sides; ++j) {
                float currentX = std::cos(longitudeIncrement * static_cast<float>(j)) * std::cos(currentLatitudeAngle) * radius;
                float currentZ = std::sin(longitudeIncrement * static_cast<float>(j)) * std::cos(currentLatitudeAngle) * radius;

                vertices.push_back({
                    {currentX, currentY, currentZ},
                    {1.0f, 0.0f, 0.0f}
                });
            }
        }

        return vertices;
    }

    inline void makeSphere(Mesh& mesh, const float radius, const int sides) {
        mesh.vertices = getSphereVertices(radius, sides);
    }
}