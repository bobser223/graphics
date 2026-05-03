#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 texCoord;

void main() {
    gl_Position = aPos;
    vertexColor = aColor;
    texCoord = aTexCoord;
}