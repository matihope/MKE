#pragma once
#include "MKE/DrawContext.hpp"
#include "MKE/Font.hpp"
#include "MKE/Font.hpp"
#include "MKE/Primitives/2d/Text.hpp"
#include "MKE/WorldEntity.hpp"

namespace mk::gui {
	enum class HAlignment { LEFT, MIDDLE, RIGHT };

	enum class VAlignment { TOP, CENTER, BOTTOM };

	class Label: public WorldEntityUI {
		Font*       m_font{};
		HAlignment  m_halignment = HAlignment::LEFT;
		VAlignment  m_valignment = VAlignment::TOP;
		Text2D      m_text;
		std::string m_string;

	public:
		Label();
		explicit Label(mk::Font* font);

		Label(mk::Font* font, const std::string& text);

		void               setFont(Font* font);
		void               setString(const std::string& text);
		const std::string& getString() const;

		void setAlignment(HAlignment newHAlignment, VAlignment newVAlignment);
		void setTextSize(unsigned int newSize);
		void setColor(Color newColor);
		void onDraw(RenderTarget& target, DrawContext context, const Game& game) const override;

		math::RectF getBounds() const;
	};
}  // namespace mk::gui
