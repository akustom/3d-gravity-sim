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

        MeshIndexData(const int vbo_o = 0, const int ebo_o = 0) : vbo_offset(vbo_o), ebo_offset(ebo_o) {}
    };

public:
    inline static std::vector<MeshIndexData>    indexedMeshes;
    inline static std::vector<gfx::vertex>      batchedVertices;
    inline static std::vector<glm::uint>        batchedIndices;

    static void initMesh(gfx::Mesh& mesh);
    static void Mesh(gfx::Mesh& mesh, int instances);
};