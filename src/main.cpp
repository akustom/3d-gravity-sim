#include <iostream>
#include <string>
#include <vector>
// opengl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// glm
#include <glm/gtc/type_ptr.hpp>
// just my libs
#include <glw/glw.hpp>
#include <gfx/gfx.hpp>
#include <physics/phy.hpp>

gfx::Camera* currentCameraPtr = nullptr;

float lastX = 480.0f; // Set to window width / 2
float lastY = 270.0f; // Set to window height / 2
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, const double xpos, const double ypos) {
    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    auto xoffset = static_cast<float>(xpos - lastX);
    auto yoffset = static_cast<float>(lastY - ypos);

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    currentCameraPtr->processMouse(xoffset, yoffset);
}

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

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
    vertexVAO.bind();

    glw::VBO squareVBO;
    squareVBO.bind();
    squareVBO.bufferData(square.vertices, GL_STATIC_DRAW);

    glw::EBO squareEBO;
    squareEBO.bind();
    squareEBO.bufferData(square.indices, GL_STATIC_DRAW);

    vertexVAO.linkAttribute(0, 3, GL_FLOAT, 6, 0); // links mesh vertices
    vertexVAO.linkAttribute(1, 3, GL_FLOAT, 6, 3); // links mesh colors

    phy::Particles particles;
    particles.createParticle(0, {0, 0, 0});

    glw::VBO instanceVBO;
    instanceVBO.bind();
    instanceVBO.bufferData(particles.positions, GL_STATIC_DRAW);

    vertexVAO.linkAttribute(2, 4, GL_DOUBLE, 4, 0); // links instances positions
    vertexVAO.setAttributeDivisor(2, 1);

    gfx::Camera camera;
    currentCameraPtr = &camera;
    glw::UBO cameraUBO;
    cameraUBO.bind(0);
    cameraUBO.allocateBuffer(8);
    camera.pushViewMatrix(cameraUBO);
    camera.pushProjectionMatrix(cameraUBO);

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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