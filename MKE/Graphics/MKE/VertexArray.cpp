#include "VertexArray.hpp"

#include "RenderTarget.hpp"

void mk::VertexArray2D::draw2d(const RenderTarget& target, DrawContext2D context) const {
	target.render(*this, context);
}
