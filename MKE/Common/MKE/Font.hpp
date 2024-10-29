#pragma once

#include "MKE/Math/Vector.hpp"
#include "MKE/NonCopyable.hpp"
#include "MKE/ResPath.hpp"
#include "MKE/Texture.hpp"

#include <unordered_map>

namespace mk {

	struct Character {
		Texture        texture{};
		math::Vector2u size{};     // size of glyph
		math::Vector2u bearing{};  // offset from baseline to left/top of glyph
		u32            advance{};  // offset to advance next glyph
	};

	struct FontParams {
		usize char_size    = 32;
		float char_scaling = 1.0;

		bool operator==(const FontParams&) const = default;

		std::size_t operator()(const FontParams& p) const noexcept {
			return std::size_t(p.char_size * p.char_scaling);
		}
	};
}

template<>
struct std::hash<mk::FontParams> {
	std::size_t operator()(const mk::FontParams& p) const noexcept {
		return std::size_t(p.char_size * p.char_scaling);
	}
};

namespace mk {
	class Font: NonCopyable {
	public:
		static constexpr usize NUM_LOAD_CHARS = 128;

		Font() = default;
		Font(const ResPath& font);
		~Font() = default;
		void load(const ResPath& font);

		using CharMap = std::array<Character, NUM_LOAD_CHARS>;

		const CharMap& getChars(const FontParams& font_params);

		void setSmooth(bool smooth);
		bool isSmooth() const;

	private:
		std::unordered_map<FontParams, CharMap> chars{};

		ResPath font_path;
		bool    loaded    = false;
		bool    is_smooth = true;
	};
}
