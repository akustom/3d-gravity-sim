#include <iostream>
#include <string>
#include <vector>
// opengl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// glm
#include <glm/gtc/type_ptr.hpp>
// just my libs
#include "glw/glw.hpp"
#include "gfx/gfx.hpp"
#include "win/win.hpp"
#include "physics/phy.hpp"
#include "io_utils.hpp"
#include "util.hpp"
#include "fps.hpp"


int main() {
    glfwInit();

    win::Window window{
        4, 6,
        960, 540, "gravity sim babyyyy"
    };
    window.use();

    glfwSetFramebufferSizeCallback(window.glfw_window, [](GLFWwindow* win, const int w, const int h) {
        glViewport(0, 0, w, h);
    });

    window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    window.setCursorPosCallback(io::mouse_callback);
    window.setMouseButtonCallback(io::mouse_button_callback);

    gladLoadGL();

    glViewport(0, 0, 960, 540);

    glw::ShaderProgram shaderProgram;
    shaderProgram.build(
        {SOURCE_DIR "assets/shaders/vertexShader.glsl", GL_VERTEX_SHADER},
        {SOURCE_DIR "assets/shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER}
    );
    shaderProgram.use();

    gfx::Mesh square;
    gfx::makePolyhedron(square, 1.0f, 8);

    glw::VAO vertexVAO;
    vertexVAO.formatAttribute(0, 0, 3, GL_FLOAT, offsetof(gfx::vertex, pos));
    vertexVAO.formatAttribute(1, 0, 3, GL_FLOAT, offsetof(gfx::vertex, color));

    vertexVAO.formatAttribute(2, 1, 4, GL_DOUBLE, 0);
    vertexVAO.setAttributeDivisor(1, 1);

    glw::VBO squareVBO;
    squareVBO.allocateBuffer(square.vertices);
    vertexVAO.attachBuffer(squareVBO, 0, 0, bytesof<gfx::vertex>());

    glw::EBO squareEBO;
    squareEBO.allocateBuffer(square.indices);
    vertexVAO.attachBuffer(squareEBO);

    phy::Particles particles;
    particles.createParticle(0, {0, 0, 0});

    glw::VBO instanceVBO;
    instanceVBO.allocateBuffer(particles.positions);
    vertexVAO.attachBuffer(instanceVBO, 1, 0, bytesof<glm::dvec4>());

    gfx::Camera camera;
    glw::UBO cameraUBO;
    cameraUBO.bind(0);
    cameraUBO.allocateBuffer(2 * bytesof<glm::mat4>());
    camera.use(window, cameraUBO);

    vertexVAO.bind();

    FrameTimer frameTimer;

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window.glfw_window)) {
        glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float dt = frameTimer.getFrameTime();

        gfx::drawInstances(static_cast<int>(particles.positions.size()), square);
        camera.processKeyboard(window, dt);
        camera.pushViewMatrix(cameraUBO);

        glfwSwapBuffers(window.glfw_window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window.glfw_window);
    glfwTerminate();
    return 0;
}