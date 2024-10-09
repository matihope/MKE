#version 410 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
out vec2 tex_coords;

uniform mat4 camera;
uniform mat4 transform;

void main() {
    gl_Position = camera * vec4(pos, 0.0, 1.0);
    tex_coords = tex;
}
