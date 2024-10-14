#pragma once

#include "MKE/Drawable.hpp"
#include "MKE/Font.hpp"
#include "MKE/Transformable.hpp"

namespace mk {
	class Text: public Drawable2D, public Transformable {
	public:
		Text() = default;

		void setText(const std::string& text);

	private:
        std::string text;
		mk::Font* font;
	};
}
