#pragma once

#include "MKE/Math/Vector.hpp"
#include "MKE/NonCopyable.hpp"
#include "MKE/ResPath.hpp"
#include "MKE/Texture.hpp"

#include <unordered_map>

namespace mk {
	class Font: NonCopyable {
	public:
		static constexpr usize NUM_LOAD_CHARS = 128;

		Font() = default;
		Font(const ResPath& font);
		~Font() = default;
		void load(const ResPath& font);

		struct Character {
			Texture        texture{};
			math::Vector2u size{};     // size of glyph
			math::Vector2u bearing{};  // offset from baseline to left/top of glyph
			u32            advance{};  // offset to advance next glyph
		};

		using CharMap = std::array<Character, NUM_LOAD_CHARS>;

		struct FontParams {
			usize char_size    = 32;
			float char_scaling = false;

			bool operator==(const FontParams&) const = default;

			std::size_t operator()(const mk::Font::FontParams& p) const noexcept {
				return std::size_t(p.char_size * p.char_scaling);
			}
		};

		const CharMap& getChars(const FontParams& font_params);

	private:
		// Float, because font_size * scaling is the key.
		// FontParams has it's own hashing method
		std::unordered_map<FontParams, CharMap, FontParams> chars{};

		ResPath font_path;
		bool    loaded = false;
	};
}
