#include "VertexArray.hpp"

#include "RenderTarget.hpp"

void mk::VertexArray2D::draw2d(const RenderTarget&, DrawContext2D context) const {
	if (!vertex_buffer_size) return;
	// Context has to be bound at the last possible moment (after all the changes could've been
	// done).
	context.bind();
	startDraw();
}

void mk::VertexArray3D::draw3d(const RenderTarget&, DrawContext3D context) const {
	if (!vertex_buffer_size) return;
	// Context has to be bound at the last possible moment (after all the changes could've been
	// done).
	context.bind();
	startDraw();
}
