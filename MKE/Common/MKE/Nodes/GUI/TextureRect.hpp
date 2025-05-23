#pragma once
#include "Label.hpp"
#include "MKE/WorldEntity.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"

namespace mk::gui {

	class TextureRect final: public WorldEntityUI {
	public:
		explicit TextureRect(const Texture* texture = nullptr);

		void onDraw(RenderTarget& target, DrawContext context, const Game& game) const override;

		void setTexture(const Texture* texture);

		math::RectF getBounds() const;

		void setAlignment(HAlignment newHAlignment, VAlignment newVAlignment);


	private:
		void realign();
		// TODO: Make alignments the a WorldEntityUI thing by adding getBounds as a default thing?
		VAlignment v_alignment;
		HAlignment h_alignment;
		RectPrimitive  rect;
		const Texture* texture = nullptr;
	};
}
