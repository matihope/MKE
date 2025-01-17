#pragma once
#include "MKE/WorldEntity.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"

namespace mk::gui {

	class TextureRect final: public WorldEntityUI {
	public:
		explicit TextureRect(const Texture* texture);

		void onDraw(RenderTarget& target, DrawContext context, const Game& game) const override;

	private:
		RectPrimitive  rect;
		const Texture* texture = nullptr;
	};
}
