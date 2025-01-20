#pragma once
#include "MKE/WorldEntity.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"

namespace mk::gui {
	class ColorRect: public WorldEntityUI {
	public:
		ColorRect(math::Vector2f size, Color color);
		void setColor(Color color);
		void onDraw(RenderTarget& target, DrawContext context, const Game& game) const override;

	private:
		RectPrimitive rect;
	};
}
