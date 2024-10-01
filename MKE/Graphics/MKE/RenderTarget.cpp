#include "RenderTarget.hpp"
#include "Drawable.hpp"

void mk::RenderTarget::render(const Drawable& drawable) const { drawable.beginDraw(*this); }

void mk::RenderTarget::render(const Drawable2D& drawable, DrawContext2D context) const {
	drawable.draw(*this, context);
}

void mk::RenderTarget::render(const VertexArray2D& vertex_array, DrawContext2D context) const {
	context.bind();
	vertex_array.start_draw();
}
