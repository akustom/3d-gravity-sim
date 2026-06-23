#pragma once

#include <vector>
#include <glm/glm.hpp>


namespace gfx {
    struct vertex;

    struct Mesh {
        std::vector<vertex> vertices;

        std::vector<glm::uvec3> indices;

        void loadModel();
    };

    /**turns your Mesh into a regular polygon of n-sides*/
    void makePolygon(Mesh& mesh, float radius, int sides);

    std::vector<vertex> getPolyhedronVertices(float radius, int sides);

    std::vector<glm::uvec3> getPolyhedronIndices(int sides);

    void makePolyhedron(Mesh& mesh, float radius, int sides);
}