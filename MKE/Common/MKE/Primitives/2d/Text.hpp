#pragma once

#include "MKE/Color.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Font.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/NonCopyable.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"
#include "MKE/Math/Rect.hpp"
#include "MKE/RenderTexture.hpp"
#include "MKE/Transformable.hpp"

namespace mk {
	class Text2D: public Drawable, public Transformable, public NonCopyable {
	public:
		Text2D() = default;

		void               setFont(mk::Font* font);
		void               setString(const std::string& text);
		const std::string& getString() const;

		void  setCharacterSize(usize size);
		usize getCharacterSize() const;
		void  setCharacterScaling(float scale);

		void setColor(mk::Color color);

		void draw(const RenderTarget& target, DrawContext context) const override;

		math::RectF getLocalBounds() const;
		math::RectF getGlobalBounds() const;

	private:
		void render();
		bool isEmpty() const;

		mk::Font*                font{};
		mk::Color                color = Colors::WHITE;
		const mk::Font::CharMap* chars = nullptr;

		std::string text;
		usize       char_size    = 32;
		float       char_scaling = 1.;

		math::RectF     text_bounds;
		RenderTexture2D render_texture;
		RectPrimitive   render_object{ { 1.f, 1.f } };
	};
}
