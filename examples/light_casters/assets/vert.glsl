#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTex;

uniform mat4 transform;
uniform mat4 camera;
uniform vec3 lightPos;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

void main() {
    vec4 pos = transform * vec4(aPos, 1.0);
    gl_Position = camera * pos;

    fragPos = pos.xyz;
    normal = mat3(transpose(inverse(transform))) * aNormal;

    texCoords = aTex;
}
