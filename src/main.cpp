#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

struct Shader {
    GLuint id = 0;
    std::string sourceCode;
    const char* source = nullptr;

    void loadSource(const std::string& filePath) {
        sourceCode = readFile(filePath);
        source = sourceCode.c_str();
    }
};

struct ShaderProgram {
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

public:             // im gonna hardcode the shaders for now. also, its not like im gonna need more than one program anyway
    GLuint id = 0;  // but, if i ever do, this architecture would make it easy to refactor

    Shader vertexShader;
    Shader fragmentShader;

    void compile() {
        vertexShader.loadSource(SOURCE_DIR "/shaders/vertexShader.vert");
        fragmentShader.loadSource(SOURCE_DIR "/shaders/fragmentShader.frag");

        compileVert(vertexShader);
        compileFrag(fragmentShader);
    }

    void build() {
        id = glCreateProgram();
        glAttachShader(id, vertexShader.id);
        glAttachShader(id, fragmentShader.id);
        glLinkProgram(id);
    }

    void use() const {
        glUseProgram(id);
    }
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

    ShaderProgram shaderProgram;

    shaderProgram.compile();
    shaderProgram.build();
    shaderProgram.use();

    float vertices[] =
    {
        0.0f,  0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
       -0.25f, -0.25f, 0.5f, 0.0f, 0.1f, 1.0f,
        0.25f, -0.25f, 0.5f, 0.0f, 0.1f, 1.0f,
   };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)nullptr); //  mesh position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // mesh colors
    glEnableVertexAttribArray(1);

    float instancePos[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    GLuint instancedVBO;
    glGenBuffers(1, &instancedVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instancePos), instancePos, GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glVertexAttribDivisor(2, 1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(VAO);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, sizeof(instancePos)/(3 * sizeof(float)));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}