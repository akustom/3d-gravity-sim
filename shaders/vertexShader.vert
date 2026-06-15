#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aOffset;
out vec3 vertexColor;

void main() {
    vec3 modifiedPos = aPos + aOffset;
    gl_Position = vec4(modifiedPos, 1.0);
    vertexColor = aColor;
}