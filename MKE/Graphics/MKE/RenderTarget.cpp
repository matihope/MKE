#include "RenderTarget.hpp"
#include "VertexArray.hpp"
#include "Drawable.hpp"

void mk::RenderTarget::render(const Drawable& drawable) const { drawable.draw(*this); }

void mk::RenderTarget::render(const Drawable2D& drawable, DrawContext2D context) const {
	drawable.draw2d(*this, context);
}

void mk::RenderTarget::render(const VertexArray2D& vertex_array, DrawContext2D context) const {
	if (!vertex_array.vertex_buffer_size) return;
	// Context has to be bound at the last possible moment (after all the changes could've been
	// done).
	context.bind();
	// context.shader->setMatrix4f("camera", camera_transform);
	vertex_array.startDraw();
}

void mk::RenderTarget::setCamera(math::Matrix4f camera_transform) {
	this->camera_transform = camera_transform;
}
