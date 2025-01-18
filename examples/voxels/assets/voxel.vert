#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec4 aColor;

uniform mat4 transform;
uniform mat4 camera;

out vec4 ourColor;
out vec3 pixelPos;
out vec2 TexCoord;
void main()
{
    gl_Position = camera * transform * vec4(aPos, 1.0);
    TexCoord = aTex;
    ourColor = aColor;
    pixelPos = (transform * vec4(aPos, 1.0)).xyz;
}
