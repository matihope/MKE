//
// Created by mateusz on 8/30/23.
//

#include "RectShape.hpp"
#include "MKE/WorldEntity.hpp"

namespace mk {
	RectShape::RectShape(Color color, mk::math::Vector2f size): WorldEntity2D(), rect(size) {
		rect.setColor(color);
	}

	void RectShape::setColor(Color color) { rect.setColor(color); }

	void RectShape::setSize(const mk::math::Vector2f& size) { rect.setSize(size); }

	mk::math::Vector2f RectShape::getSize() const { return rect.getSize(); }

	void RectShape::onDraw(RenderTarget& target, DrawContext context, const Game&) const {
		context.transform *= getTransform();
		if (texture) context.texture = texture;
		target.render(rect, context);
	}

	RectShape::RectShape(): RectShape(Colors::WHITE, { 0, 0 }) {}

	void RectShape::setTexture(const mk::Texture* texture) { this->texture = texture; }
}  // namespace mk
