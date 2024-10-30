#include "RenderTexture.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Exceptions.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Texture.hpp"
#include "glad/glad.h"

void mk::RenderTexture2D::create(usize width, usize height) { create({ width, height }); }

mk::RenderTexture2D::~RenderTexture2D() {
	glDeleteFramebuffers(1, &frame_buffer_id);
	glDeleteRenderbuffers(1, &render_buffer_object);
}

void mk::RenderTexture2D::render(const Drawable& drawable, DrawContext context) {
	if (size.x == 0 || size.y == 0) return;

	context.camera(1, 1) *= -1;
	context.camera(1, 3) *= -1;

	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);
	glViewport(0, 0, size.x * scaling_factor.x, size.y * scaling_factor.y);
	defaultRender(drawable, context);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const mk::Texture& mk::RenderTexture2D::getTexture() const { return texture; }

u32 mk::RenderTexture2D::getFBO() const { return frame_buffer_id; }

void mk::RenderTexture2D::clear(mk::Color color) const {
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void mk::RenderTexture2D::setSmooth(bool smooth) { texture.setSmooth(smooth); }

void mk::RenderTexture2D::setScalingFactor(math::Vector2f scaling_factor) {
	this->scaling_factor = scaling_factor;
	regenerateBuffers();
}

mk::RenderTexture2D::RenderTexture2D() {
	glGenFramebuffers(1, &frame_buffer_id);
	glGenRenderbuffers(1, &render_buffer_object);
}

void mk::RenderTexture2D::regenerateBuffers() {
	auto [scaled_width, scaled_height] = (size.type<float>() * scaling_factor).bind();

	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);

	texture.loadFromMemory(scaled_width, scaled_height, nullptr, GL_RGBA);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getNativeHandle(), 0
	);

	glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_object);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, scaled_width, scaled_height);
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer_object
	);

	if (auto creation_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	    creation_status != GL_FRAMEBUFFER_COMPLETE)
		throw exceptions::MkException("Framebuffer creation error: ", creation_status);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void mk::RenderTexture2D::create(math::Vector2u size) {
	this->size = size;
	regenerateBuffers();
}

mk::math::Vector2u mk::RenderTexture2D::getSize() const {
	return size;
}

mk::math::Vector2f mk::RenderTexture2D::getScalingFactor() const { return scaling_factor; }
