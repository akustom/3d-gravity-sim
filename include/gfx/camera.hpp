#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glw/buffer.hpp"


const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  5.0f;
const float SENSITIVITY =  0.1f;
const float FOV         =  45.0f;


namespace gfx {
    struct Camera {
        glm::vec3 position  = {0.0f, 0.0f, 5.0f};

        glm::vec3 worldUp = {0.0f, 1.0f, 0.0f};
        glm::vec3 front   = {0.0f, 0.0f, -1.0f};
        glm::vec3 right   = glm::normalize(glm::cross(front, worldUp));
        glm::vec3 up      = glm::normalize(glm::cross(right, front));

        float yaw   = YAW;
        float pitch = PITCH;

        float movementSpeed     = SPEED;
        float mouseSensitivity  = SENSITIVITY;
        float fieldOfView       = FOV;

        bool isViewDirty = true;

        void use(GLFWwindow *window, const glw::UBO& camera_ubo) {
            glfwSetWindowUserPointer(window, this);
            camera_ubo.bind(0);
            camera_ubo.allocateBuffer(8);
            pushViewMatrix(camera_ubo);
            pushProjectionMatrix(camera_ubo);
        }

        void processKeyboard(GLFWwindow *window, const float& dt) {
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

        void processMouse(double x_offset, double y_offset, GLboolean constrain_pitch = true) {
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

        [[nodiscard]] glm::mat4 getViewMatrix() const {
            return glm::lookAt(position, position + front, up);
        }

        void pushViewMatrix(const glw::UBO& ubo) {
            if (!isViewDirty)
                return;
            ubo.pushUniform(0, getViewMatrix());
            isViewDirty = false;
        }

        void pushProjectionMatrix(const glw::UBO& ubo) const {
            ubo.pushUniform(4, glm::perspective(glm::radians(fieldOfView), 960.0f/540.0f, 0.1f, 100.0f));
        }

    private:
        void updateCameraVectors() {
            glm::vec3 tempFront;
            tempFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            tempFront.y = sin(glm::radians(pitch));
            tempFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            front = glm::normalize(tempFront);
            right = glm::normalize(glm::cross(tempFront, worldUp));
            up    = glm::normalize(glm::cross(right, tempFront));
        }
    };
}
