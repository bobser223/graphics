#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vertexColor;

void main() {
    gl_Position = aPos;
    vertexColor = aColor;
}