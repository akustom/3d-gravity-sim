#pragma once

#include <iostream>
#include <glm/gtc/type_ptr.hpp>


template <typename T>
    concept trivially_copyable = std::is_trivially_copyable_v<T>;

template<typename T>
constexpr auto getPtr(const T& data) {
    if constexpr (std::is_same_v<T,glm::mat4>)
        return glm::value_ptr(data);
    else
        throw std::invalid_argument("uniform type is currently unsupported\n");
}