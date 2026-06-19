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
#include <gl_utilities.hpp>
#include <utilities.hpp>

struct Camera {
    glm::vec3 position = {0.0f, 0.0f, 5.0f};
    glm::vec3 target = {0.0f, 0.0f, 0.0f};

    glm::vec3 cameraFront = glm::normalize(position - target);
    glm::vec3 cameraRight = glm::cross({0.0f, 1.0f, 0.0f}, cameraFront);
    glm::vec3 cameraUp = glm::cross(cameraFront, cameraRight);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 960.0f/540.0f, 0.1f, 100.0f);

    bool isViewDirty = true;

    void posAdd(const glm::vec3& displace) {
        position += displace;
        isViewDirty = !isViewDirty;
    }

    [[nodiscard]] glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, target, cameraUp);
    }

    void pushViewMatrix(const glu::ShaderProgram& program, const char* view_uniform) const {
        if (!isViewDirty)
            return;
        GLint viewLoc = glGetUniformLocation(program.id, view_uniform);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(getViewMatrix()));
    }

    void pushProjectionMatrix(const glu::ShaderProgram& program, const char* projection_uniform) {
        GLint projectionLoc = glGetUniformLocation(program.id, projection_uniform);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }
};

struct Mesh {
    std::vector<util::vertex> vertices = {
        {{-0.25f, 0.25f, 0.0f},     {1.0f, 0.0f, 0.0f}},
        {{-0.25f, -0.25f, 0.0f},    {0.0f, 1.0f, 0.0f}},
        {{0.25f, -0.25f, 0.0f},     {0.0f, 0.0f, 1.0f}},
        {{0.25f, 0.25f, 0.0f},      {1.0f, 0.0f, 0.0f}},
        {{-0.25f, 0.25f, 0.0f},     {1.0f, 0.0f, 0.0f}},
        {{0.25f, -0.25f, 0.0f},     {0.0f, 0.0f, 1.0f}},
    };
};


struct Particles {
    std::vector<glm::dvec3> instancePos = {
        {0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {-1.0, 0.0, 0.0}
    };
};

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

    glu::Shader vertex{SOURCE_DIR "shaders/vertexShader.vert", GL_VERTEX_SHADER};
    glu::Shader fragment{SOURCE_DIR "shaders/fragmentShader.frag", GL_FRAGMENT_SHADER};

    glu::ShaderProgram shaderProgram;
    shaderProgram.build(vertex, fragment);
    shaderProgram.use();

    Mesh triangle;

    glu::VAO vertexVAO;
    vertexVAO.bind();

    glu::VBO triangleVBO;
    triangleVBO.bind();
    triangleVBO.bufferData(triangle.vertices, GL_STATIC_DRAW);

    vertexVAO.linkAttribute(0, 3, GL_FLOAT, 6, 0); // links mesh vertices
    vertexVAO.linkAttribute(1, 3, GL_FLOAT, 6, 3); // links mesh colors

    Particles particles;

    glu::VBO instanceVBO;
    instanceVBO.bind();
    instanceVBO.bufferData(particles.instancePos, GL_STATIC_DRAW);

    vertexVAO.linkAttribute(2, 3, GL_DOUBLE, 3, 0);
    vertexVAO.setAttributeDivisor(2, 1);

    Camera camera;
    GLint viewLoc = glGetUniformLocation(shaderProgram.id, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
    GLint projectionLoc = glGetUniformLocation(shaderProgram.id, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(particles.instancePos.size()));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}