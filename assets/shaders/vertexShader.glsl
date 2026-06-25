#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

layout (location = 3) in vec4 displacement;
layout (std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
};

out vec3 vertexColor;
out vec3 vertexNormal;

void main() {
    gl_Position = projection * view * (vec4(aPos, 0.0) + displacement);
    vertexColor = aColor;
    vertexNormal = aNormal;
}