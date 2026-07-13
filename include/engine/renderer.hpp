#pragma once

#include "gfx/mesh.hpp"

void drawInstances(int instance_count, const gfx::Mesh& mesh);
void drawInstancesBaseVertex(int instance_count, const gfx::Mesh& mesh, int ebo_offset, int vbo_offset);