#include "VertexArray.hpp"
#include "MKE/Shaders/SimpleShader.hpp"

void mk::VertexArray2D::draw2d(const RenderTarget2D&, DrawContext context) const {
	if (!vertex_buffer_size) return;
	// Context has to be bound at the last possible moment (after all the changes could've been
	// done).
	if (!context.shader) {
		if (context.texture)
			context.shader = shaders::shader2D();
		else
			context.shader = shaders::shaderNoTexture2D();
	}
	context.bind();
	startDraw();
}

void mk::VertexArray3D::draw3d(const RenderTarget3D&, DrawContext context) const {
	if (!vertex_buffer_size) return;
	// Context has to be bound at the last possible moment (after all the changes could've been
	// done).
	if (!context.shader) {
		if (context.texture)
			context.shader = shaders::shader3D();
		else
			context.shader = shaders::shaderNoTexture3D();
	}
	context.bind();
	startDraw();
}
