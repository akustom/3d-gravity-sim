#pragma once

#include <iostream>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>


template <typename T>
    concept trivially_copyable = std::is_trivially_copyable_v<T>;

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