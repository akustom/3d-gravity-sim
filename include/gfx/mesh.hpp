#pragma once

#include <vector>
#include <glm/glm.hpp>


namespace gfx {
    struct vertex;

    struct Mesh {
        std::vector<vertex> vertices;
        std::vector<glm::uint> indices;

        int vertexCount;
        int indexCount;

        void loadModel();
    };

    /**turns your Mesh into a regular polygon of n-sides*/
    void makePolygon(Mesh& mesh, float radius, int sides);

    std::vector<vertex> getPolyhedronVertices(float radius, int sides, const glm::vec3& color);

    std::vector<glm::uint> getPolyhedronIndices(int sides);

    void getPolyhedronNormals(std::vector<vertex>& vertices);

    void makePolyhedron(Mesh& mesh, float radius, int sides, const glm::vec3& color);
}