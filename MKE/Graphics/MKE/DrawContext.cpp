#include "DrawContext.hpp"

namespace {
	const char* shader_vertex2d   = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
out vec3 ourColor;
out vec2 TexCoord;
void main()
{
   gl_Position = vec4(aPos, 1.0);
   ourColor = aColor;
   TexCoord = aTex;
}
	      )";
	const char* shader_fragment2d = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
out vec3 ourColor;
out vec2 TexCoord;
void main()
{
   gl_Position = vec4(aPos, 1.0);
   ourColor = aColor;
   TexCoord = aTex;
}
	      )";

	mk::Shader* shader2D() {
		static mk::Shader shader(shader_vertex2d, shader_fragment2d);
		return &shader;
	}
}

mk::DrawContext2D::DrawContext2D() { setShader(shader2D()); }
