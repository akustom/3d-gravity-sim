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
#include <vector3.hpp>


struct Camera {
    glm::vec3 position = {0.0f, 0.0f, 1.0f};
    glm::vec3 target = {0.0f, 0.0f, 0.0f};

    glm::vec3 cameraFront = glm::normalize(position - target);
    glm::vec3 cameraRight = glm::cross({0.0f, 1.0f, 0.0f}, cameraFront);
    glm::vec3 cameraUp = glm::cross(cameraFront, cameraRight);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 960.0f/540.0f, 0.1f, 100.0f);

    [[nodiscard]] glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, target, cameraUp);
    }
};


struct Mesh {
    std::vector<float> vertices = {
        -0.25f,  0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
       -0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.25f,  0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
};


struct Particles {
    std::vector<glm::vec3> instancePos = {
        {0.0f, 0.0f, 0.0f},
    };

    std::vector<glm::mat4> instanceModel;

    void updateInstanceModel() {
        instanceModel.resize(instancePos.size());
        for (int i = 0; i < instancePos.size(); i++) {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, instancePos[i]);
            instanceModel[i] = model;
        }
    }
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

    glu::ShaderProgram shaderProgram;
    shaderProgram.compileShaders();
    shaderProgram.build();
    shaderProgram.use();

    Mesh triangle;

    glu::VAO vertexVAO;
    vertexVAO.gen();
    vertexVAO.bind();

    glu::VBO triangleVBO;
    triangleVBO.gen();
    triangleVBO.bind();
    triangleVBO.bufferData(triangle.vertices, GL_STATIC_DRAW);

    vertexVAO.linkAttribute(0, 3, 6, 0); // links mesh vertices
    vertexVAO.linkAttribute(1, 3, 6, 3); // links mesh colors

    Particles particles;

    glu::VBO instanceVBO;
    instanceVBO.gen();
    instanceVBO.bind();

    particles.updateInstanceModel();

    instanceVBO.bufferData(particles.instanceModel, GL_STATIC_DRAW);
    for (int i = 0; i < 4; i++) {
        vertexVAO.linkAttribute(2 + i, 4, 16, i * 4);
        vertexVAO.setAttributeDivisor(2 + i, 1);
    }

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