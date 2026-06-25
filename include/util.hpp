#pragma once

#include <vector>


template <typename T>
constexpr size_t bytesof(std::vector<T>& vec) {
    return vec.size() * sizeof(T);
}

template <typename T>
constexpr size_t bytesof() {
    return sizeof(T);
}