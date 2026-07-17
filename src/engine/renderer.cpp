#include "engine/renderer.hpp"

#include "gfx/vertex.hpp"
#include "glw/render.hpp"
#include "util.hpp"

#include "tracy/Tracy.hpp"

void Renderer::refreshBuffers(gfx::Mesh& mesh) {
    if (verticesBatch.size + mesh.vertices.size() > verticesBatch.capacity) {
        verticesBatch.capacity = 2 * std::max(verticesBatch.size, sc_int(mesh.vertices.size()));

        glw::VBO tempVBO;
        tempVBO.allocateBuffer<gfx::vertex>(verticesBatch.capacity, GL_DYNAMIC_STORAGE_BIT);
        tempVBO.copyData<gfx::vertex>(batchedVBO, verticesBatch.size);

        batchedVBO = std::move(tempVBO);
    }
    batchedVBO.pushData(sc_int(bytesof<gfx::vertex>()) * verticesBatch.size, mesh.vertices);
    VAO.attachBuffer(batchedVBO, 0, 0, bytesof<gfx::vertex>());

    if (indicesBatch.size + mesh.indices.size() > indicesBatch.capacity) {
        indicesBatch.capacity = 2 * std::max(indicesBatch.size, sc_int(mesh.indices.size()));

        glw::EBO tempEBO;
        tempEBO.allocateBuffer<glm::uint>(indicesBatch.capacity, GL_DYNAMIC_STORAGE_BIT);
        tempEBO.copyData<glm::uint>(batchedEBO, indicesBatch.size);

        batchedEBO = std::move(tempEBO);
    }
    batchedEBO.pushData(sc_int(bytesof<glm::uint>()) * indicesBatch.size, mesh.indices);
    VAO.attachBuffer(batchedEBO);
}

void Renderer::indexMesh(gfx::Mesh& mesh) {
    ZoneScoped;
    mesh.id = sc_int(indexedMeshes.size());
    indexedMeshes.emplace_back(indicesBatch.size, verticesBatch.size);

    indexedMeshes[mesh.id].indexCount = sc_int(mesh.indices.size());
    indexedMeshes[mesh.id].ebo_offset = indicesBatch.size;
    indexedMeshes[mesh.id].vbo_offset = verticesBatch.size;

    refreshBuffers(mesh);

    indicesBatch.size  += sc_int(mesh.indices.size());
    verticesBatch.size += sc_int(mesh.vertices.size());
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