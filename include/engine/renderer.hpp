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


    glw::VBO batchedVBO;
    glw::EBO batchedEBO;

public:
    glw::VAO VAO; // TODO: move this into private once you handle line ~75 in main.cpp

    std::vector<MeshIndexData>    indexedMeshes;
    std::vector<gfx::vertex>      batchedVertices;
    std::vector<glm::uint>        batchedIndices;

    // IDEA: pre-allocate a large amount of vram for vertex data nad stuff like that, make its capacity double everytime it gets full
    // this approach prevents expensive reallocation, and it even

    Renderer() {
        VAO.formatAttribute(0, 0, 3, GL_FLOAT, offsetof(gfx::vertex, pos));
        VAO.formatAttribute(1, 0, 3, GL_FLOAT, offsetof(gfx::vertex, color));
        VAO.formatAttribute(2, 0, 3, GL_FLOAT, offsetof(gfx::vertex, normal));

        VAO.formatAttribute(3, 1, 4, GL_FLOAT, 0);
        VAO.setAttributeDivisor(1, 1);
    }

    void refreshBuffers();

    void indexMesh(gfx::Mesh& mesh);
    void Mesh(gfx::Mesh& mesh, int instances);
};
