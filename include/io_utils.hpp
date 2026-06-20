#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


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