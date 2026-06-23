#pragma once

struct GLFWwindow;


namespace win {
    struct Window {
        GLFWwindow* glfw_window;
        void setHints(int major_version, int minor_version);
        void init(int width, int height, const char* window_name);

        Window() = default;
        Window(int major_version, int minor_version, int width, int height, const char* window_name) {
            setHints(major_version, minor_version);
            init(width, height, window_name);
        }

        void use() const;

        void setInputMode(int mode, int value) const;
        void setCursorPosCallback(void(*cursor_callback)(GLFWwindow* window, double x_pos, double y_pos)) const;
        void setMouseButtonCallback(void(*mouse_button_callback)(GLFWwindow* window, int button, int action, int mods)) const;
        void setUserPointer(void* user_pointer) const;

        [[nodiscard]] bool isKeyPressed(int key) const;

        void getWindowSize(int* width, int* height) const;
    };
}
