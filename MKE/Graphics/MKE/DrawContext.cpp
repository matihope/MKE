#include "DrawContext.hpp"
#include "MKE/Shader.hpp"

void mk::DrawContext::bind() {
	if (!shader) return;

	Shader::use(shader);
	Texture::bind(texture);
	shader->setMatrix4f("transform", transform);
	shader->setMatrix4f("camera", camera);
}

mk::DrawContext::DrawContext(const Shader* shader): shader(shader) {}

mk::DrawContext::DrawContext(const Texture* texture): texture(texture) {}
