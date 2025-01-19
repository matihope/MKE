#include "Font.hpp"
#include "MKE/ResPath.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "MKE/Exceptions.hpp"
#include "MKE/Panic.hpp"
#include "glad/glad.h"

namespace {
	struct FontLib {
		FT_Library                               ft;
		std::unordered_map<mk::ResPath, FT_Face> faces;

		FontLib() {
			if (FT_Init_FreeType(&ft))
				throw mk::exceptions::MkException("Could not init FreeType Library");
		}

		FT_Face getFace(const mk::ResPath& path) {
			auto [face, new_face] = faces.try_emplace(path);
			if (new_face) {
				if (FT_New_Face(ft, path.strPath(), 0, &face->second))
					throw mk::exceptions::MkException("Failed to load font: ", path.strPath());
			}

			return face->second;
		}

		~FontLib() {
			for (auto&& [_, face]: faces) FT_Done_Face(face);
			FT_Done_FreeType(ft);
		}
	};

	FT_Face getFace(const mk::ResPath& path) {
		static FontLib lib;
		return lib.getFace(path);
	}
}

mk::Font::Font(const ResPath& font) { load(font); }

void mk::Font::load(const ResPath& font) {
	MK_ASSERT_TRUE(!loaded, "Loading into a loaded font");
	font_path = font;
	loaded    = true;
}

const mk::Font::CharMap& mk::Font::getChars(const FontParams& font_params) {
	if (!loaded) MK_PANIC("Cannot getChars() from an unloaded font...");

	// Try loading cache
	auto [char_map, new_char_map] = chars.try_emplace(font_params);

	if (new_char_map) {
		const FT_Face face = getFace(font_path);

		// else load the font
		FT_Set_Pixel_Sizes(face, 0, u32(font_params.char_size * font_params.char_scaling));

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // disable byte-alignment restriction

		for (unsigned char c = 0; c < NUM_LOAD_CHARS; c++) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				MK_LOG_ERROR(
					"Failed to load Glyph: ", int(c), " during loading font: ", font_path.getPath()
				);
				continue;
			}
			Character& character = char_map->second.at(c);

			glBindTexture(GL_TEXTURE_2D, character.texture.getNativeHandle());
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);

			character.size
				= mk::math::Vector2u(face->glyph->bitmap.width, face->glyph->bitmap.rows);

			character.bearing
				= mk::math::Vector2u(face->glyph->bitmap_left, face->glyph->bitmap_top);

			character.advance = face->glyph->advance.x;
		}
	}
	return char_map->second;
}

void mk::Font::setSmooth(bool smooth) { is_smooth = smooth; }

bool mk::Font::isSmooth() const { return is_smooth; }
