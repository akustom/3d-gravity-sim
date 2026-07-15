#include "gfx/mesh.hpp"

#include <vector>
#include <cmath>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "gfx/vertex.hpp"


namespace gfx {
    /**turns your Mesh into a regular polygon of n-sides*/
    void makePolygon(Mesh& mesh, float radius, int sides) {
        if (sides < 3) return;

        std::vector<vertex> vertices;
        std::vector<glm::uint> indices;

        vertices.reserve(sides + 1);
        vertices.push_back({{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}});

        float angleIncrement = glm::radians(360.0f / static_cast<float>(sides));

        for (int i = 0; i < sides; ++i) {
            float currentAngle = angleIncrement * static_cast<float>(i);
            glm::vec3 pos = glm::vec3(
                radius * std::cos(currentAngle),
                radius * std::sin(currentAngle),
                0.0f
            );
            vertices.push_back({pos, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}});
        }

        for (int i = 1; i <= sides; ++i) {
            GLuint center   = 0;
            GLuint current  = i;
            GLuint next     = i == sides ? 1 : i + 1;

            indices.insert(indices.end(), {center, current, next});
        }

        mesh.vertices   = vertices;
        mesh.indices    = indices;
    }

    std::vector<vertex> getPolyhedronVertices(float radius, int sides, const glm::vec3& color) {
        std::vector<vertex> vertices;

        float latitudeIncrement     = glm::radians(360.0f / static_cast<float>(sides % 2 == 0 ? sides : sides - 1));
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
                    color,
                    {0.0f, 0.0f, 0.0f}
                });
            }
        }

        return vertices;
    }

    std::vector<glm::uint> getPolyhedronIndices(int sides) {
        std::vector<glm::uint> indices;

        for (int i = 0; i < sides / 2; ++i) {
            for (int j = 0; j < sides; ++j) {
                glm::uint tri0_0 = i * sides + j;
                glm::uint tri0_1 = i * sides + (j + 1) % sides;
                glm::uint tri0_2 = (i + 1) * sides + j;

                indices.insert(indices.end(), {tri0_0, tri0_1, tri0_2});

                glm::uint tri1_0 = i * sides + (j + 1) % sides;
                glm::uint tri1_1 = (i + 1) * sides + j;
                glm::uint tri1_2 = (i + 1) * sides + (j + 1) % sides;

                indices.insert(indices.end(), {tri1_0, tri1_1, tri1_2});
            }
        }
        return indices;
    }

    void getPolyhedronNormals(std::vector<vertex>& vertices) {
        for (auto& vertex : vertices) {
            vertex.normal = glm::normalize(vertex.pos);
        }
    }

    void makePolyhedron(Mesh& mesh, float radius, int sides, const glm::vec3& color) {
        if (sides <= 3)
            return;

        mesh.vertices   = getPolyhedronVertices(radius, sides, color);
        mesh.indices    = getPolyhedronIndices(sides);
        getPolyhedronNormals(mesh.vertices);
    }
}