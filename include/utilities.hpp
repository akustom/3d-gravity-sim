#pragma once

#include <iostream>
#include <glm/glm.hpp>

namespace util {
    struct vertex {
        glm::vec3 pos;
        glm::vec3 color;

        vertex(glm::vec3 pos, glm::vec3 color) : pos(pos), color(color) {}
    };
}