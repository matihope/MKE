#include "DrawContext.hpp"

namespace {
	const char* shader_vertex2d   = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTex;
uniform mat4 transform;
out vec4 ourColor;
out vec2 TexCoord;
void main()
{
   gl_Position = transform * vec4(aPos, 0.0, 1.0);
   ourColor = aColor;
   TexCoord = aTex;
}
	      )";
	const char* shader_fragment2d = R"(
#version 330 core
in vec4 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
   FragColor = texture(ourTexture, TexCoord) * ourColor;
}
	      )";

	mk::Shader* shader2D() {
		static mk::Shader shader(shader_vertex2d, shader_fragment2d);
		return &shader;
	}
}

mk::DrawContext2D::DrawContext2D() { setShader(shader2D()); }

void mk::DrawContext2D::setShader(const Shader* shader) { DrawContext::setShader(shader); }

mk::DrawContext2D::DrawContext2D(const Texture& texture): DrawContext2D() {
	this->texture = &texture;
}
