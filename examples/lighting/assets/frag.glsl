#version 410 core

in vec4 color;
in vec2 texCoors;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
    FragColor = vec4(objectColor * lightColor, 1.0);
}
