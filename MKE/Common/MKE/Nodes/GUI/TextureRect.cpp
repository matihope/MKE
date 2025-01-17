#include "TextureRect.hpp"

namespace mk::gui {
	TextureRect::TextureRect(const Texture* texture):
		  rect(texture ? texture->getSize().type<float>() : math::Vector2f{ 1.f }),
		  texture(texture) {}

	void TextureRect::onDraw(RenderTarget& target, DrawContext context, const Game& game) const {
		if (texture) {
			context.transform *= getTransform();
			context.texture = texture;
			target.render(rect, context);
		}
	}
}
