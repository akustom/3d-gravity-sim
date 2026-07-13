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

#include "engine/fps.hpp"
#include "engine/renderer.hpp"


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
    gfx::makePolyhedron(square, 1.0f, 64, {1.0, 1.0, 1.0});

    gfx::Mesh cube;
    gfx::makePolyhedron(cube, 1.0f, 64, {1.0, 0.0, 0.0});

    gfx::Mesh combined;

    moveVec(combined.vertices, square.vertices, cube.vertices);
    moveVec(combined.indices, square.indices, cube.indices);

    glw::VAO vertexVAO;
    vertexVAO.formatAttribute(0, 0, 3, GL_FLOAT, offsetof(gfx::vertex, pos));
    vertexVAO.formatAttribute(1, 0, 3, GL_FLOAT, offsetof(gfx::vertex, color));
    vertexVAO.formatAttribute(2, 0, 3, GL_FLOAT, offsetof(gfx::vertex, normal));

    vertexVAO.formatAttribute(3, 1, 4, GL_DOUBLE, 0);
    vertexVAO.setAttributeDivisor(1, 1);

    glw::VBO meshesVBO;
    meshesVBO.allocateBuffer(combined.vertices);
    vertexVAO.attachBuffer(meshesVBO, 0, 0, bytesof<gfx::vertex>());

    glw::EBO meshesEBO;
    meshesEBO.allocateBuffer(combined.indices);
    vertexVAO.attachBuffer(meshesEBO);

    phy::Particles particles;
    particles.createParticle(0, {0, 0, 0});
    particles.createParticle(0, {2, 0, 0});
    particles.createParticle(0, {-2, 0, 0});
    particles.createParticle(0, {0, 2, 0});
    particles.createParticle(0, {0, -2, 0});
    particles.createParticle(0, {0, 0, 2});
    particles.createParticle(0, {0, 0, -2});

    glw::VBO instanceVBO;
    instanceVBO.allocateBuffer(particles.positions);
    vertexVAO.attachBuffer(instanceVBO, 1, 0, bytesof<glm::dvec4>());

    gfx::Camera camera;
    glw::UBO cameraUBO;
    cameraUBO.bind(0);
    cameraUBO.allocateBuffer(2 * bytesof<glm::mat4>());
    camera.use(window, cameraUBO);

    vertexVAO.bind();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    while (!glfwWindowShouldClose(window.glfw_window)) {
        if (FrameTimer::SetFPS(120))
            continue;

        float dt = FrameTimer::getFrameTime();

        glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawInstancesBaseVertex(static_cast<int>(particles.positions.size()), cube, square.indexCount, square.vertexCount);
        drawInstancesBaseVertex(static_cast<int>(particles.positions.size()), square, 0, 0);

        camera.processKeyboard(window, dt);
        camera.pushViewMatrix(cameraUBO);

        glfwSwapBuffers(window.glfw_window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window.glfw_window);
    glfwTerminate();
    return 0;
}