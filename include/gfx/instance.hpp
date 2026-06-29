#pragma once

#include "mesh.hpp"

namespace gfx {
    void drawInstances(int instance_count, const Mesh& mesh);
    void drawInstancesBaseVertex(int instance_count, const Mesh& mesh, int ebo_offset, int vbo_offset);
}