#include "MKE/DrawContext.hpp"
#include "Label.hpp"

namespace mk::gui {
	Label::Label() { m_text.setCharacterSize(32); }

	Label::Label(Font* font): Label() { setFont(font); }

	Label::Label(Font* font, const std::string& text): Label(font) { setString(text); }

	void Label::setFont(Font* font) {
		m_text.setFont(font);
		m_font = font;
		setTextSize(m_text.getCharacterSize());
	}

	void Label::setString(const std::string& newText) {
		m_text.setString(newText);
		m_string = newText;
		setAlignment(m_halignment, m_valignment);
	}

	void Label::setAlignment(HAlignment newHAlignment, VAlignment newVAlignment) {
		m_halignment   = newHAlignment;
		m_valignment   = newVAlignment;
		int textWidth  = (int) m_text.getLocalBounds().width;
		int textHeight = (int) m_text.getLocalBounds().height;

		// int newlines = (int) std::ranges::count(m_string, '\n');
		//		textHeight *= newlines + 1;

		math::Vector2f newPos(-m_text.getLocalBounds().left, -m_text.getLocalBounds().top);
		switch (m_halignment) {
		case HAlignment::LEFT:
			break;
		case HAlignment::MIDDLE:
			newPos.x -= (float) textWidth / 2;
			break;
		case HAlignment::RIGHT:
			newPos.x -= (float) textWidth;
			break;
		}
		switch (m_valignment) {
		case VAlignment::TOP:
			//			newPos.y += (float) textHeight;
			break;
		case VAlignment::CENTER:
			newPos.y -= (float) textHeight / 2;
			break;
		case VAlignment::BOTTOM:
			newPos.y -= (float) textHeight;
			break;
		}
		m_text.setPosition((int) newPos.x, (int) newPos.y);
	}

	void Label::onDraw(RenderTarget& target, DrawContext context, const Game&) const {
		if (m_text.getString() == "") return;
		context.transform *= getTransform();
		target.render(m_text, context);
	}

	void Label::setTextSize(const unsigned int newSize) {
		m_text.setCharacterSize(newSize);
		setAlignment(m_halignment, m_valignment);
		// m_text.setCharacterScaling(2.0);
	}

	void Label::setColor(const mk::Color newColor) { m_text.setColor(newColor); }

	math::RectF Label::getBounds() const {
		auto bounds = m_text.getGlobalBounds();
		bounds.left += getPosition2D().x;
		bounds.top += getPosition2D().y;
		return bounds;
	}

	const std::string& Label::getString() const { return m_string; }
}  // namespace mk::GUI
