//
// Created by Mateusz Kolpa on 20/01/2025.
//

#include "ColorRect.hpp"

namespace mk::gui {
	ColorRect::ColorRect(const math::Vector2f size, const Color color): rect(size) {
		rect.setColor(color);
	}

	void ColorRect::setColor(const Color color) { rect.setColor(color); }

	void ColorRect::onDraw(RenderTarget& target, DrawContext context, const Game& game) const {
		WorldEntityUI::onDraw(target, context, game);
		context.transform *= getTransform();
		target.render(rect, context);
	}
}
