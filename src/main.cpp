#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str().c_str();
}

struct Shader {
    GLuint id{};
    const char* source{};

    void loadSource(const std::string& filePath) {
        source = readFile(filePath);
    }
};

struct ShaderManager {
private:
    static void compileVert(Shader& shader) {
        shader.id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader.id, 1, &shader.source, nullptr);
        glCompileShader(shader.id);
    }

    static void compileFrag(Shader& shader) {
        shader.id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader.id, 1, &shader.source, nullptr);
        glCompileShader(shader.id);
    }

public: // im gonna hardcode the shaders for now. its not like i need more than one pair rn
    Shader vertexShader;
    Shader fragmentShader;
};


int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Hello World", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "failed to create GLFW window" << "\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, 800, 800);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
}