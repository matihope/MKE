#version 410 core

in vec2 tex_coords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main() {
    color = vec4(textColor.xzy, textColor.a * texture(text, tex_coords).r);
}
