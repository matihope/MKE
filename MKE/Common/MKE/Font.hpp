#pragma once

#include "MKE/Math/Vector.hpp"
#include "MKE/NonCopyable.hpp"
#include "MKE/ResPath.hpp"
#include "MKE/Texture.hpp"

#include <ft2build.h>
#include <unordered_map>
#include FT_FREETYPE_H

namespace mk {
	class Font: NonCopyable {
	public:
		Font() = default;
		Font(const ResPath& font);
		~Font();
		void load(const ResPath& font);

		void setSize(u32 font_size);
		void setScaling(float scaling);

		// Make those private
		struct Character {
			Texture        texture{};
			math::Vector2u size{};     // size of glyph
			math::Vector2u bearing{};  // offset from baseline to left/top of glyph
			u32            advance{};  // offset to advance next glyph
		};

		std::unordered_map<char8_t, Character> chars;
	private:

		ResPath    font_path;
		FT_Library ft;
		FT_Face    face;
		bool       loaded    = false;
		u32        font_size = 48;
		float      scaling   = 1.f;

		void reload();
	};
}
