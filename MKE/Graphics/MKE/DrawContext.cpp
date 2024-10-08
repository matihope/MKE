#include "DrawContext.hpp"
#include "MKE/Shader.hpp"

namespace {
	const char* shader_vertex2d = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTex;
uniform mat4 transform;
uniform mat4 camera;
out vec4 ourColor;
out vec2 TexCoord;
void main()
{
   gl_Position = camera * transform * vec4(aPos, 0.0, 1.0);
   ourColor = aColor;
   TexCoord = aTex;
}
	      )";

	const char* shader_vertex3d = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTex;
uniform mat4 transform;
uniform mat4 camera;
out vec4 ourColor;
out vec2 TexCoord;
void main()
{
   gl_Position = camera * transform * vec4(aPos, 1.0);
   ourColor = aColor;
   TexCoord = aTex;
}
	      )";

	const char* shader_fragment = R"(
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
		static mk::Shader shader(shader_vertex2d, shader_fragment);
		return &shader;
	}

	mk::Shader* shader3D() {
		static mk::Shader shader(shader_vertex3d, shader_fragment);
		return &shader;
	}

	const char* shader_fragment_no_texture = R"(
#version 330 core
in vec4 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

void main()
{
   FragColor = ourColor;
}
	      )";

	mk::Shader* shaderNoTexture2D() {
		static mk::Shader shader(shader_vertex2d, shader_fragment_no_texture);
		return &shader;
	}

	mk::Shader* shaderNoTexture3D() {
		static mk::Shader shader(shader_vertex3d, shader_fragment_no_texture);
		return &shader;
	}
}

void mk::DrawContext::bind() {
	if (!shader) return;

	Shader::use(shader);
	Texture::bind(texture);
	shader->setMatrix4f("transform", transform);
	shader->setMatrix4f("camera", camera);
}

mk::DrawContext::DrawContext(Shader* shader): shader(shader) {}

mk::DrawContext::DrawContext(Texture* texture): texture(texture) {}

void mk::DrawContext2D::bind() {
	if (texture)
		shader = shader2D();
	else
		shader = shaderNoTexture2D();
	DrawContext::bind();
}

void mk::DrawContext3D::bind() {
	if (texture)
		shader = shader3D();
	else
		shader = shaderNoTexture3D();
	DrawContext::bind();
}
