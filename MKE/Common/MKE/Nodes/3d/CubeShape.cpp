#include "CubeShape.hpp"

void mk::CubeShape::onDraw(RenderTarget& target, DrawContext context, const Game&) const {
	context.transform *= getTransform();
	target.render(cube, context);
}
