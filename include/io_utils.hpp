#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
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

    inline float lastX = 480.0f;
    inline float lastY = 270.0f;
    inline bool firstMouse = true;

    inline void mouse_callback(GLFWwindow* window, const double xpos, const double ypos) {
        if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
            return;

        if (firstMouse) {
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);
            firstMouse = false;
        }

        const auto xoffset = static_cast<float>(xpos - lastX);
        const auto yoffset = static_cast<float>(lastY - ypos);

        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);

        if (auto* camera = static_cast<gfx::Camera*>(glfwGetWindowUserPointer(window))) {
            camera->processMouse(xoffset, yoffset);
        }
    }

    inline void mouse_button_callback(GLFWwindow* window, const int button, const int action, const int mods) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            if (action == GLFW_PRESS) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                firstMouse = true;
            }
            else if (action == GLFW_RELEASE) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
    }
}