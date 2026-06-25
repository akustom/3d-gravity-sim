#version 460 core
out vec4 FragColor;
in vec3 vertexColor;
in vec3 vertexNormal;

void main() {
    FragColor = vec4(vertexColor, 1.0f);
}