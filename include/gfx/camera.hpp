#pragma once

#include <cmath>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

#include <glm/fwd.hpp>

#include "window.hpp"


namespace glw {
    struct UBO;
}

namespace gfx {
    struct Camera {
        glm::vec3 position  = {0.0f, 0.0f, 5.0f};

        glm::vec3 worldUp = {0.0f, 1.0f, 0.0f};
        glm::vec3 front   = {0.0f, 0.0f, -1.0f};
        glm::vec3 right   = glm::normalize(glm::cross(front, worldUp));
        glm::vec3 up      = glm::normalize(glm::cross(right, front));

        float yaw   = glm::degrees(std::atan2(front.z, front.x));
        float pitch = glm::degrees(std::asin(front.y));

        float movementSpeed     = 5.0f;
        float mouseSensitivity  = 0.1f;
        float fieldOfView       = 45.0f;

        win::Window window;

        float lastX = 0.0f, lastY = 0.0f;
        bool firstMouse = true;

        bool isViewDirty = true;

        void use(const win::Window& window, const glw::UBO& camera_ubo);

        void processKeyboard(const win::Window& window, float dt);

        void processMouse(double x_offset, double y_offset, bool constrain_pitch = true);

        [[nodiscard]] glm::mat4 getViewMatrix() const;

        void pushViewMatrix(const glw::UBO& ubo);

        void pushProjectionMatrix(const glw::UBO& ubo) const;

    private:
        void updateCameraVectors();
    };
}
