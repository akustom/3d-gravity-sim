#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl_utilities.hpp>

struct Mesh {
    std::vector<float> vertices = {
        0.0f,  0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
       -0.25f, -0.25f, 0.5f, 0.0f, 0.1f, 1.0f,
        0.25f, -0.25f, 0.5f, 0.0f, 0.1f, 1.0f,
    };
};

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Hello World", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, 800, 800);

    gl::ShaderProgram shaderProgram;
    shaderProgram.compileShaders();
    shaderProgram.build();
    shaderProgram.use();

    Mesh triangle;

    gl::VAO vertexVAO;
    vertexVAO.gen();
    vertexVAO.bind();

    gl::VBO triangleVBO;
    triangleVBO.gen();
    triangleVBO.bind();
    triangleVBO.bufferData(triangle.vertices, GL_STATIC_DRAW);

    vertexVAO.linkAttribute(0, 3, 6, 0); // links mesh vertices
    vertexVAO.linkAttribute(1, 3, 6, 3); // links mesh colors

    std::vector instancePos{
        0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    gl::VBO instanceVBO;
    instanceVBO.gen();
    instanceVBO.bind();
    instanceVBO.bufferData(instancePos, GL_STATIC_DRAW);

    vertexVAO.linkAttribute(2, 3, 3, 0);
    vertexVAO.setAttributeDivisor(2, 1);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, static_cast<GLsizei>(instancePos.size() * sizeof(float)/(3 * sizeof(float))));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}