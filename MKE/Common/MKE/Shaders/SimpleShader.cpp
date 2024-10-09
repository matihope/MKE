#include "SimpleShader.hpp"

namespace {
	const char* shader_vertex2d = R"(
#version 410 core
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
#version 410 core
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
#version 410 core
in vec4 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
   FragColor = texture(ourTexture, TexCoord) * ourColor;
}
	      )";


	const char* shader_fragment_no_texture = R"(
#version 410 core
in vec4 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

void main()
{
   FragColor = ourColor;
}
	      )";

}

namespace mk::shaders {
	Shader* shader2D() {
		static mk::Shader shader(shader_vertex2d, shader_fragment);
		return &shader;
	}

	Shader* shader3D() {
		static mk::Shader shader(shader_vertex3d, shader_fragment);
		return &shader;
	}

	Shader* shaderNoTexture2D() {
		static mk::Shader shader(shader_vertex2d, shader_fragment_no_texture);
		return &shader;
	}

	Shader* shaderNoTexture3D() {
		static mk::Shader shader(shader_vertex3d, shader_fragment_no_texture);
		return &shader;
	}
}
