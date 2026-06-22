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
        vertices.push_back({{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}});

        float angleIncrement = glm::radians(360.0f / static_cast<float>(sides));

        for (int i = 0; i < sides; ++i) {
            float currentAngle = angleIncrement * static_cast<float>(i);
            glm::vec3 pos = glm::vec3(
                radius * std::cos(currentAngle),
                radius * std::sin(currentAngle),
                0.0f
            );
            vertices.push_back({pos, {1.0f, 1.0f, 1.0f}});
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

        float latitudeIncrement     = glm::radians(360.0f / static_cast<float>(sides));
        float longitudeIncrement    = glm::radians(360.0f / static_cast<float>(sides));

        for (int i = 0; i <= sides / 2; ++i) {
            float currentLatitudeAngle      = -glm::half_pi<float>() + latitudeIncrement * static_cast<float>(i);
            float currentLongitudeLength    = std::cos(currentLatitudeAngle);

            float currentY = std::sin(currentLatitudeAngle) * radius;

            for (int j = 0; j < sides; ++j) {
                float currentX = std::cos(longitudeIncrement * static_cast<float>(j)) * currentLongitudeLength * radius;
                float currentZ = std::sin(longitudeIncrement * static_cast<float>(j)) * currentLongitudeLength * radius;

                vertices.push_back({
                    {currentX, currentY, currentZ},
                    {1.0f, 0.0f, 0.0f}
                });
            }
        }

        return vertices;
    }

    inline std::vector<glm::uvec3> getSphereIndices(const int sides) {
        std::vector<glm::uvec3> indices;

        for (int i = 0; i < sides; ++i) {
            for (int j = 0; j < sides; ++j) {
                glm::uvec3 tri1Quad = {
                    i * sides + j,
                    i * sides + j + 1,
                    (i + 1) * sides + j
                };
                glm::uvec3 tri2Quad = {
                    i * sides + j + 1,
                    (i + 1) * sides + j,
                    (i + 1) * sides + j + 1,
                };

                indices.push_back(tri1Quad);
                indices.push_back(tri2Quad);
            }
        }
        return indices;
    }

    inline void makeSphere(Mesh& mesh, const float radius, const int sides) {
        for (auto& index : getSphereIndices(sides)) {
            std::cout << '[' << index.x << ", " << index.y << ", " << index.z << "]\n";
        }
        mesh.vertices = getSphereVertices(radius, sides);
        mesh.indices = getSphereIndices(sides);
    }
}