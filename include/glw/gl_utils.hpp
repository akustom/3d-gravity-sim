#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


template <typename T>
    concept trivially_copyable = std::is_trivially_copyable_v<T>;

template<typename T>
constexpr auto getPtr(const T& data) {
    if constexpr (std::is_same_v<T,glm::mat4>)
        return glm::value_ptr(data);
    else
        std::cerr << "uniform type is currently unsupported\n";
};

constexpr int getGLTypeSize(const GLenum type) {
    switch (type) {
        case GL_FLOAT:  return sizeof(float);
        case GL_INT:    return sizeof(int);
        case GL_DOUBLE: return sizeof(double);
        default:
            throw std::invalid_argument("unexpected gl type");
    }
}