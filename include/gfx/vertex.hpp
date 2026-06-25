#pragma once

#include <glm/vec3.hpp>


namespace gfx {
    struct vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec3 normal;

        vertex(glm::vec3 pos, glm::vec3 color, glm::vec3 normal) : pos(pos), color(color), normal(normal) {}
    };
}