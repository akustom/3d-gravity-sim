#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glw/glw.hpp>

namespace gfx {
    struct Camera {
        glm::vec3 position = {0.0f, 0.0f, 5.0f};
        glm::vec3 target = {0.0f, 0.0f, 0.0f};

        glm::vec3 cameraFront = glm::normalize(position - target);
        glm::vec3 cameraRight = glm::cross({0.0f, 1.0f, 0.0f}, cameraFront);
        glm::vec3 cameraUp = glm::cross(cameraFront, cameraRight);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 960.0f/540.0f, 0.1f, 100.0f);

        bool isViewDirty = true;

        void posAdd(const glm::vec3& displace) {
            position += displace;
            isViewDirty = true;
        }

        [[nodiscard]] glm::mat4 getViewMatrix() const {
            return glm::lookAt(position, target, cameraUp);
        }

        void pushViewMatrix(const glw::UBO& ubo) const {
            if (!isViewDirty)
                return;
            ubo.pushUniform(0, getViewMatrix());
        }

        void pushProjectionMatrix(const glw::UBO& ubo) const {
            ubo.pushUniform(4, projection);
        }
    };
}
