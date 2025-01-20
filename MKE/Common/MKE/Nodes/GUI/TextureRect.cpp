#include "TextureRect.hpp"

namespace mk::gui {
	TextureRect::TextureRect(const Texture* texture): rect(math::Vector2f{ 1.f }) {
		setTexture(texture);
	}

	void TextureRect::onDraw(RenderTarget& target, DrawContext context, const Game& game) const {
		if (texture) {
			context.transform *= getTransform();
			context.texture = texture;
			target.render(rect, context);
		}
	}

	void TextureRect::setTexture(const Texture* texture) {
		if (texture)
			rect.setSize(texture->getSize().type<float>());
		else
			rect.setSize(math::Vector2f{ 1.f });
		this->texture = texture;
		realign();
	}

	math::RectF TextureRect::getBounds() const {
		auto       top_left     = math::Vector2f(0.f);
		auto       bottom_right = top_left + rect.getSize();
		const auto transform    = getTransform();
		top_left                = transform ^ top_left;
		bottom_right            = transform ^ bottom_right;
		if (bottom_right.x < top_left.x) std::swap(bottom_right.x, top_left.x);
		if (bottom_right.y < top_left.y) std::swap(bottom_right.y, top_left.y);
		return { top_left.x, top_left.y, bottom_right.x - top_left.x, bottom_right.y - top_left.y };
	}

	void TextureRect::setAlignment(HAlignment newHAlignment, VAlignment newVAlignment) {
		v_alignment = newVAlignment;
		h_alignment = newHAlignment;
		realign();
	}

	void TextureRect::realign() {
		auto [width, height] = getBounds().getSize().vec_data;

		math::Vector2f offset{};
		switch (v_alignment) {
		case VAlignment::TOP:
			break;
		case VAlignment::CENTER:
			offset.y = height / 2;
			break;
		case VAlignment::BOTTOM:
			offset.y = height;
			break;
		}

		switch (h_alignment) {
		case HAlignment::LEFT:
			break;
		case HAlignment::MIDDLE:
			offset.x = width / 2;
			break;
		case HAlignment::RIGHT:
			offset.x = width;
			break;
		}

		rect.setPosition(-offset);
	}
}
