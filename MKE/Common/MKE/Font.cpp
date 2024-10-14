#include "Font.hpp"
#include "MKE/Exceptions.hpp"
#include "MKE/Panic.hpp"
#include "glad/glad.h"

mk::Font::Font(const ResPath& font) { load(font); }

void mk::Font::load(const ResPath& font) {
	MK_ASSERT_TRUE(!loaded, "Loading into a loaded font");
	font_path = font;

	if (FT_Init_FreeType(&ft)) throw exceptions::MkException("Could not init FreeType Library");

	if (FT_New_Face(ft, font.strPath(), 0, &face))
		throw exceptions::MkException("Failed to load font: ", font.strPath());

	loaded = true;
	reload();
}

mk::Font::~Font() {
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void mk::Font::setSize(u32 font_size) {
	this->font_size = font_size;
	reload();
}

void mk::Font::reload() {
	if (loaded) {
		FT_Set_Pixel_Sizes(face, 0, font_size * scaling);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // disable byte-alignment restriction

		for (unsigned char c = 0; c < 128; c++) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				MK_LOG_ERROR(
					"Failed to load Glyph: ", int(c), " during loading font: ", font_path.getPath()
				);
				continue;
			}

			chars.try_emplace(c);
			Character& character = chars.at(c);

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
				= mk::math::Vector2u(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				character.bearing
				= mk::math::Vector2u(face->glyph->bitmap_left, face->glyph->bitmap_top),
				character.advance = face->glyph->advance.x;
		}
	}
}

void mk::Font::setScaling(float scaling) { this->scaling = scaling; }
