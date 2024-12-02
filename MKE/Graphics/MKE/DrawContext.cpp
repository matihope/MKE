#include "DrawContext.hpp"
#include "MKE/Shader.hpp"

void mk::DrawContext::bind() {
	if (!shader) return;

	Shader::use(shader);
	Texture::bindAt(texture, 0);
	shader->setMatrix4f("transform", transform);
	shader->setMatrix4f("camera", camera);
}

mk::DrawContext::DrawContext(const Shader* shader): shader(shader) {}

mk::DrawContext::DrawContext(const Texture* texture): texture(texture) {}

mk::DrawContext::DrawContext(const math::Matrix4f& camera) { this->camera = camera; }
