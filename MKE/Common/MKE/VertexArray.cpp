#include "VertexArray.hpp"
#include "MKE/Shaders/SimpleShader.hpp"

void mk::VertexArray2D::draw(RenderTarget& target, DrawContext context) const {
	if (!context.shader) {
		if (context.texture)
			context.shader = shaders::shader2D();
		else
			context.shader = shaders::shaderNoTexture2D();
	}
	VertexArray::draw(target, context);
}

void mk::VertexArray3D::draw(RenderTarget& target, DrawContext context) const {
	if (!context.shader) {
		if (context.texture)
			context.shader = shaders::shader3D();
		else
			context.shader = shaders::shaderNoTexture3D();
	}
	VertexArray::draw(target, context);
}
