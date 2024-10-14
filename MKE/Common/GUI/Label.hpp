#pragma once
#include "MKE/Font.hpp"

namespace mk::gui {
	enum class HAlignment { LEFT, MIDDLE, RIGHT };

	enum class VAlignment { TOP, CENTER, BOTTOM };

	class Label: public WorldEntity {
		sf::Font*   m_font{};
		HAlignment  m_halignment = HAlignment::LEFT;
		VAlignment  m_valignment = VAlignment::TOP;
		sf::Text    m_text;
		std::string m_string;

	public:
		Label();
		explicit Label(mk::Font* font);

		Label(mk::Font* font, const std::string& text);

		void          setFont(Font* font);
		void          setText(const std::string& text);
		void          setAlignment(HAlignment newHAlignment, VAlignment newVAlignment);
		void          setTextSize(unsigned int newSize);
		void          setColor(Color newColor);
		void          onDraw(RenderTarget& target, RenderStates states) const override;
		RectF getBounds() const;

		const std::string& getText() const;
	};
}  // namespace mk::gui
