#pragma once

#include <vector>

#include "glm/fwd.hpp"

#include "glw/buffer.hpp"
#include "gfx/vertex.hpp"
#include "gfx/mesh.hpp"
#include "glw/vao.hpp"


class Renderer {
    struct MeshIndexData {
        int indexCount = 0;

        int ebo_offset;
        int vbo_offset;

        MeshIndexData(const int ebo_o = 0, const int vbo_o = 0) : ebo_offset(ebo_o), vbo_offset(vbo_o) {}
    };

    void refreshBuffers();

public:
    std::vector<MeshIndexData>    indexedMeshes;
    std::vector<gfx::vertex>      batchedVertices;
    std::vector<glm::uint>        batchedIndices;

    glw::VBO batchedVBO;
    glw::EBO batchedEBO;

    glw::VAO VAO;

    void initMesh(gfx::Mesh& mesh);
    void Mesh(gfx::Mesh& mesh, int instances);
};
