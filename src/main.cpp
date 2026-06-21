#include <iostream>
#include <string>
#include <vector>
// opengl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// just my utilities
#include <glw/glw.hpp>
#include <gfx/gfx.hpp>


int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(960, 540, "Hello World", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, 960, 540);

    glw::ShaderProgram shaderProgram;
    shaderProgram.build(
        {SOURCE_DIR "assets/shaders/vertexShader.glsl", GL_VERTEX_SHADER},
        {SOURCE_DIR "assets/shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER}
    );
    shaderProgram.use();

    gfx::Mesh square;
    gfx::makeSquare(square);
    gfx::makePolygon(1.0f, 64, square);

    glw::VAO vertexVAO;
    vertexVAO.bind();

    glw::VBO squareVBO;
    squareVBO.bind();
    squareVBO.bufferData(square.vertices, GL_STATIC_DRAW);

    glw::EBO squareEBO;
    squareEBO.bind();
    squareEBO.bufferData(square.indices, GL_STATIC_DRAW);

    vertexVAO.linkAttribute(0, 3, GL_FLOAT, 6, 0); // links mesh vertices
    vertexVAO.linkAttribute(1, 3, GL_FLOAT, 6, 3); // links mesh colors

    gfx::Instances particles;

    glw::VBO instanceVBO;
    instanceVBO.bind();
    instanceVBO.bufferData(particles.instancePos, GL_STATIC_DRAW);

    vertexVAO.linkAttribute(2, 4, GL_DOUBLE, 4, 0);
    vertexVAO.setAttributeDivisor(2, 1);

    gfx::Camera camera;
    glw::UBO cameraUBO;
    cameraUBO.bind(0);
    cameraUBO.allocateBuffer(8);
    camera.pushViewMatrix(cameraUBO);
    camera.pushProjectionMatrix(cameraUBO);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(3 * square.indices.size()), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(particles.instancePos.size()));
        camera.position = glm::angleAxis(glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * camera.position;
        camera.pushViewMatrix(cameraUBO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}