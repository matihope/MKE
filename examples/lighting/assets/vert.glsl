#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTex;

uniform mat4 transform;
uniform mat4 camera;

out vec4 color;
out vec2 texCoords;

void main() {
    gl_Position = camera * transform * vec4(aPos, 1.0);

    color = aColor;
    texCoords = aTex;
}
