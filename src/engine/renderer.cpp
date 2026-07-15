#include "engine/renderer.hpp"

#include "glw/render.hpp"
#include "util.hpp"


void Render::initMesh(gfx::Mesh& mesh) {
    mesh.id = static_cast<int>(indexedMeshes.size());
    indexedMeshes.emplace_back(static_cast<int>(batchedVertices.size()), static_cast<int>(batchedIndices.size()));

    indexedMeshes[mesh.id].indexCount = static_cast<int>(mesh.indices.size());
    indexedMeshes[mesh.id].ebo_offset = static_cast<int>(batchedIndices.size());
    indexedMeshes[mesh.id].vbo_offset = static_cast<int>(batchedVertices.size());

    moveVecHelper(batchedIndices, mesh.indices);
    moveVecHelper(batchedVertices, mesh.vertices);
}

void Render::Mesh(gfx::Mesh& mesh, const int instances) {
    glw::drawInstancesBaseVertex(
        instances,
        indexedMeshes[mesh.id].indexCount,
        indexedMeshes[mesh.id].ebo_offset,
        indexedMeshes[mesh.id].vbo_offset
        );
}