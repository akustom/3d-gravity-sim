#pragma once

#include "../GLFW/glfw3.h"


class FrameTimer {
private:
    inline static float lastFrame = 0;
    inline static float currentFrame = 0;

public:
    static float getFrameTime() {
        const float dt    = currentFrame - lastFrame;
        lastFrame           = static_cast<float>(glfwGetTime());

        return dt;
    }

    static bool SetFPS(float fps) {
        if (static_cast<float>(glfwGetTime()) - currentFrame >= 1.0f/fps) {
            currentFrame = static_cast<float>(glfwGetTime());
            return false;
        }
        return true;
    }
};
