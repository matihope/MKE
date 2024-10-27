//
// Created by mateusz on 8/30/23.
//

#include "RectShape.hpp"

namespace mk {
	RectShape::RectShape(Color color, mk::math::Vector2f size): rect(size) { rect.setColor(color); }

	void RectShape::setColor(Color color) { rect.setColor(color); }

	void RectShape::setSize(const mk::math::Vector2f& size) { rect.setSize(size); }

	mk::math::Vector2f RectShape::getSize() const { return rect.getSize(); }

	void RectShape::onDraw(RenderTarget& target, DrawContext context, const Game&) const {
		context.transform *= getTransform();
		target.renderContext(rect, context);
	}

	RectShape::RectShape(): RectShape(Colors::WHITE, { 0, 0 }) {}
}  // namespace mk
