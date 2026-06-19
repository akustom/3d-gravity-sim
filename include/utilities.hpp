#pragma once

#include <glm/glm.hpp>

namespace util {
    struct vertex {
        glm::vec4 pos;
        glm::vec3 color;

        vertex(glm::vec3 pos, glm::vec3 color) : pos(pos, 1.0f), color(color) {}
    };
}