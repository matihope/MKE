#include "RenderTarget.hpp"
#include "VertexArray.hpp"
#include "Drawable.hpp"

void mk::RenderTarget::render(const Drawable& drawable) const { drawable.draw(*this); }

void mk::RenderTarget::render(const Drawable2D& drawable, DrawContext2D context) const {
	drawable.draw2d(*this, context);
}

void mk::RenderTarget::render(const VertexArray2D& vertex_array, DrawContext2D context) const {
	if (!vertex_array.vertex_buffer_size) return;
	context.bind();
	vertex_array.startDraw();
}
