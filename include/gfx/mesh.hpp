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

    std::vector<vertex> getPolyhedraVertices(float radius, int sides);

    std::vector<glm::uvec3> getPolyhedraIndices(int sides);

    void makePolyhedra(Mesh& mesh, float radius, int sides);
}