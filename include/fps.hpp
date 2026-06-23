#pragma once

#include "GLFW/glfw3.h"


struct FrameTimer {
    float currentFrame = 0;
    float lastFrame = 0;

    float getFrameTime() {
        lastFrame = currentFrame;
        currentFrame = static_cast<float>(glfwGetTime());

        return currentFrame - lastFrame;
    }
};
