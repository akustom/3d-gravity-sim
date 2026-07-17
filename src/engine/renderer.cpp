#include "engine/renderer.hpp"

#include "gfx/vertex.hpp"
#include "glw/render.hpp"
#include "util.hpp"

#include "tracy/Tracy.hpp"

void Renderer::refreshBuffers() {
    batchedVBO.destroy();
    batchedVBO.create();
    batchedVBO.allocateBuffer(batchedVertices);
    VAO.attachBuffer(batchedVBO, 0, 0, bytesof<gfx::vertex>());

    batchedEBO.destroy();
    batchedEBO.create();
    batchedEBO.allocateBuffer(batchedIndices);
    VAO.attachBuffer(batchedEBO);
}

void Renderer::indexMesh(gfx::Mesh& mesh) {
    mesh.id = static_cast<int>(indexedMeshes.size());
    indexedMeshes.emplace_back(static_cast<int>(batchedVertices.size()), static_cast<int>(batchedIndices.size()));

    indexedMeshes[mesh.id].indexCount = static_cast<int>(mesh.indices.size());
    indexedMeshes[mesh.id].ebo_offset = static_cast<int>(batchedIndices.size());
    indexedMeshes[mesh.id].vbo_offset = static_cast<int>(batchedVertices.size());

    moveVecHelper(batchedIndices, mesh.indices);
    moveVecHelper(batchedVertices, mesh.vertices);

    refreshBuffers();
}

void Renderer::Mesh(gfx::Mesh& mesh, const int instances) {
    if (mesh.id == -1)
        indexMesh(mesh);

    VAO.bind();

    glw::drawInstancesBaseVertex(
        instances,
        indexedMeshes[mesh.id].indexCount,
        indexedMeshes[mesh.id].ebo_offset,
        indexedMeshes[mesh.id].vbo_offset
        );
}