#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>
#include "gfx/camera.hpp"

namespace io {
    inline std::string readFile(const std::string& file_path) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "failed to open file: " << file_path << "\n";
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    inline void mouse_callback(GLFWwindow* window, const double x_pos, const double y_pos) {
        if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
            return;

        auto* camera = static_cast<gfx::Camera*>(glfwGetWindowUserPointer(window));
        if (!camera) return;

        if (camera->firstMouse) {
            camera->lastX = static_cast<float>(x_pos);
            camera->lastY = static_cast<float>(y_pos);
            camera->firstMouse = false;
        }

        const auto x_offset = static_cast<float>(x_pos - camera->lastX);
        const auto y_offset = static_cast<float>(camera->lastY - y_pos);

        camera->lastX = static_cast<float>(x_pos);
        camera->lastY = static_cast<float>(y_pos);

        camera->processMouse(x_offset, y_offset);
    }

    inline void mouse_button_callback(GLFWwindow* window, const int button, const int action, const int mods) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            if (action == GLFW_PRESS) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                if (auto* camera = static_cast<gfx::Camera*>(glfwGetWindowUserPointer(window))) {
                    camera->firstMouse = true;
                }
            }
            else if (action == GLFW_RELEASE) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
    }
}