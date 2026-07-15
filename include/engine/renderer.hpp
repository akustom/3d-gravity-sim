#pragma once

#include <vector>

#include "glm/fwd.hpp"

#include "glw/buffer.hpp"
#include "gfx/vertex.hpp"
#include "gfx/mesh.hpp"


class Render {
    struct MeshIndexData {
        int indexCount = 0;

        int ebo_offset;
        int vbo_offset;

        MeshIndexData(const int ebo_o = 0, const int vbo_o = 0) : ebo_offset(ebo_o), vbo_offset(vbo_o) {}
    };

public:
    std::vector<MeshIndexData>    indexedMeshes;
    std::vector<gfx::vertex>      batchedVertices;
    std::vector<glm::uint>        batchedIndices;

    void initMesh(gfx::Mesh& mesh);
    void Mesh(gfx::Mesh& mesh, int instances);
};