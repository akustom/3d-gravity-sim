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

#include "engine/fps.hpp"
#include "engine/renderer.hpp"

#include "io_utils.hpp"
#include "util.hpp"


int main() {
    glfwInit();

    win::Window window{
        4, 6,
        960, 540, "gravity sim babyyyy"
    };
    window.use();
    glfwSwapInterval(0);

    glfwSetFramebufferSizeCallback(window.glfw_window, [](GLFWwindow* win, const int w, const int h) {
        glViewport(0, 0, w, h);
    });

    window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    window.setCursorPosCallback(io::mouse_callback);
    window.setMouseButtonCallback(io::mouse_button_callback);


    gladLoadGL();

    glViewport(0, 0, 960, 540);


    Render Render;


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

    Render.initMesh(square);
    Render.initMesh(cube);

    glw::VAO vertexVAO;
    vertexVAO.formatAttribute(0, 0, 3, GL_FLOAT, offsetof(gfx::vertex, pos));
    vertexVAO.formatAttribute(1, 0, 3, GL_FLOAT, offsetof(gfx::vertex, color));
    vertexVAO.formatAttribute(2, 0, 3, GL_FLOAT, offsetof(gfx::vertex, normal));

    vertexVAO.formatAttribute(3, 1, 4, GL_FLOAT, 0);
    vertexVAO.setAttributeDivisor(1, 1);


    glw::VBO meshesVBO;
    meshesVBO.allocateBuffer(Render.batchedVertices);
    vertexVAO.attachBuffer(meshesVBO, 0, 0, bytesof<gfx::vertex>());

    glw::EBO meshesEBO;
    meshesEBO.allocateBuffer(Render.batchedIndices);
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
    vertexVAO.attachBuffer(instanceVBO, 1, 0, bytesof<glm::vec4>());


    gfx::Camera camera;
    glw::UBO cameraUBO;
    cameraUBO.bind(0);
    cameraUBO.allocateBuffer(2 * bytesof<glm::mat4>());
    camera.use(window, cameraUBO);

    vertexVAO.bind();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    while (!glfwWindowShouldClose(window.glfw_window)) {
        if (!FrameTimer::SetFPS(120))
            continue;

        float dt = FrameTimer::getFrameTime();

        glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.processKeyboard(window, dt);
        camera.pushViewMatrix(cameraUBO);

        Render.Mesh(square, static_cast<int>(particles.positions.size()));

        glfwSwapBuffers(window.glfw_window);
        glfwPollEvents();

        std::cout << 1/dt << "\n";
    }

    glfwTerminate();
    return 0;
}