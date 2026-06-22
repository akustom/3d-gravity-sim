#include <iostream>
#include <string>
#include <vector>
// opengl
#include <glad/glad.h>

// glm
#include <glm/gtc/type_ptr.hpp>
// just my libs
#include "glw/glw.hpp"
#include "gfx/gfx.hpp"
#include "physics/phy.hpp"
#include "io_utils.hpp"


int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(960, 540, "Hello World", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, const int w, const int h) {
        glViewport(0, 0, w, h);
    });

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, io::mouse_callback);
    glfwSetMouseButtonCallback(window, io::mouse_button_callback);

    gladLoadGL();

    glViewport(0, 0, 960, 540);

    glw::ShaderProgram shaderProgram;
    shaderProgram.build(
        {SOURCE_DIR "assets/shaders/vertexShader.glsl", GL_VERTEX_SHADER},
        {SOURCE_DIR "assets/shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER}
    );
    shaderProgram.use();

    gfx::Mesh square;
    gfx::makePolygon(square, 1.0f, 64);

    glw::VAO vertexVAO;

    vertexVAO.formatAttribute(0, 0, 3, GL_FLOAT, offsetof(gfx::vertex, pos));
    vertexVAO.formatAttribute(1, 0, 3, GL_FLOAT, offsetof(gfx::vertex, color));

    vertexVAO.formatAttribute(2, 1, 4, GL_FLOAT, 0);
    vertexVAO.setAttributeDivisor(1, 1);

    glw::VBO squareVBO;
    squareVBO.allocateBuffer(square.vertices);
    vertexVAO.attachBuffer(squareVBO, 0, 0, square.vertices);

    glw::EBO squareEBO;
    squareEBO.allocateBuffer(square.indices);
    vertexVAO.attachBuffer(squareEBO);

    phy::Particles particles;
    particles.createParticle(0, {0, 0, 0});

    glw::VBO instanceVBO;
    instanceVBO.allocateBuffer(particles.positions);
    vertexVAO.attachBuffer(instanceVBO, 1, 0, particles.positions);

    gfx::Camera camera;
    glw::UBO cameraUBO;
    cameraUBO.bind(0);
    cameraUBO.allocateBuffer(8);
    camera.use(window, cameraUBO);

    vertexVAO.bind();

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        gfx::drawInstances(particles.positions, square);
        camera.processKeyboard(window, deltaTime);
        camera.pushViewMatrix(cameraUBO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}