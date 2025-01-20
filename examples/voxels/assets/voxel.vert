#version 410 core

layout (location = 0) in ivec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec4 aColor;

uniform ivec3 chunk_position;
uniform mat4 transform;
uniform mat4 camera;

out vec4 ourColor;
out vec3 pixelPos;  // We want to interpolate this
out vec2 TexCoord;
void main()
{
    gl_Position = camera * vec4(chunk_position + aPos, 1.0);
    TexCoord = aTex;
    ourColor = aColor;
    pixelPos = chunk_position + aPos;
}
