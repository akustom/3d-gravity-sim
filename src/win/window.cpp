#include "win/window.hpp"

#include <iostream>
#include <GLFW/glfw3.h>


namespace win {
    void Window::setHints(int major_version, int minor_version) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void Window::init(int width, int height, const char* window_name) {
        glfw_window = glfwCreateWindow(width, height, window_name, nullptr, nullptr);
        if (glfw_window == nullptr) {
            std::cout << "failed to create GLFW window\n";
            glfwTerminate();
        }
    }

    void Window::use() const {
        glfwMakeContextCurrent(glfw_window);
    }

    void Window::setInputMode(int mode, int value) const {
        glfwSetInputMode(glfw_window, mode, value);
    }

    void Window::setCursorPosCallback(void(*cursor_callback)(GLFWwindow* window, double x_pos, double y_pos)) const {
        glfwSetCursorPosCallback(glfw_window, cursor_callback);
    }

    void Window::setMouseButtonCallback(void(* mouse_button_callback)(GLFWwindow* window, int button, int action, int mods)) const {
        glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);
    }

    void Window::setUserPointer(void* user_pointer) const {
        glfwSetWindowUserPointer(glfw_window, user_pointer);
    }

    bool Window::isKeyPressed(int key) const {
        if (glfwGetKey(glfw_window, key) == GLFW_PRESS)
            return true;
        return false;
    }

    void Window::getWindowSize(int* width, int* height) const {
        glfwGetFramebufferSize(glfw_window, width, height);
    };
}
