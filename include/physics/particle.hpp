#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace phy {
    struct Particles {
        std::vector<glm::vec4> positions;
        std::vector<glm::vec4> velocities;
        std::vector<glm::vec4> accelerations;

        std::vector<double> masses;

        void createParticle(const double mass, const glm::vec3& position) {
            positions.emplace_back(position, 1);
            velocities.emplace_back(0, 0, 0, 0);
            accelerations.emplace_back(0, 0, 0, 0);

            masses.push_back(mass);
        }
    };
}