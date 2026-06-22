#include "camera.hpp"

#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glw/buffer.hpp"

namespace gfx {
    void Camera::use(GLFWwindow* window, const glw::UBO& camera_ubo) {
        this->window = window;
        glfwSetWindowUserPointer(window, this);
        pushViewMatrix(camera_ubo);
        pushProjectionMatrix(camera_ubo);
    }

    void Camera::processKeyboard(GLFWwindow* window, float dt) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            position += front * movementSpeed * dt;
            isViewDirty = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            position -= front * movementSpeed * dt;
            isViewDirty = true;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            position -= right * movementSpeed * dt;
            isViewDirty = true;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            position += right * movementSpeed * dt;
            isViewDirty = true;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            position += worldUp * movementSpeed * dt;
            isViewDirty = true;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            position -= worldUp * movementSpeed * dt;
            isViewDirty = true;
        }
    }

    void Camera::processMouse(double x_offset, double y_offset, bool constrain_pitch) {
        if (x_offset == 0 && y_offset == 0)
            return;

        isViewDirty = true;

        x_offset *= mouseSensitivity;
        y_offset *= mouseSensitivity;

        yaw   += static_cast<float>(x_offset);
        pitch += static_cast<float>(y_offset);

        if (constrain_pitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();
    }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void Camera::pushViewMatrix(const glw::UBO& ubo) {
        if (!isViewDirty)
            return;
        ubo.pushUniform(0, getViewMatrix());
        isViewDirty = false;
    }

    void Camera::pushProjectionMatrix(const glw::UBO& ubo) const {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ubo.pushUniform(
        4, glm::perspective(glm::radians(fieldOfView),
        static_cast<float>(width)/static_cast<float>(height),
        0.1f, 100.0f));
    }

    void Camera::updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(newFront, worldUp));
        up    = glm::normalize(glm::cross(right, newFront));
    }
}
