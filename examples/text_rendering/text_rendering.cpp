#include "MKE/Color.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/RenderTarget.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/ResPath.hpp"
#include "MKE/Shader.hpp"
#include "MKE/Texture.hpp"
#include "MKE/VertexArray.hpp"
#include "glad/glad.h"

#include <ft2build.h>
#include <unordered_map>
#include FT_FREETYPE_H

struct FontVertex {
	mk::math::Vector2f position;
	mk::math::Vector2f tex_coords;

	static void configureVertexAttribute() {
		glVertexAttribPointer(
			0, sizeof(position) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(FontVertex), (void*) 0
		);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(
			1,
			sizeof(tex_coords) / sizeof(float),
			GL_FLOAT,
			GL_FALSE,
			sizeof(FontVertex),
			(void*) (sizeof(position))
		);
		glEnableVertexAttribArray(1);
	}
};

class FontArray: public mk::VertexArray<FontVertex>, public mk::Drawable2D {
public:
	using mk::VertexArray<FontVertex>::VertexArray;
	~FontArray() = default;

	mk::Color    color;
	mk::Texture* texture;

	void draw2d(const mk::RenderTarget2D&, mk::DrawContext context) const override {
		static mk::Shader shader(mk::ResPath("vert.vs"), mk::ResPath("frag.fs"));
		context.shader  = &shader;
		context.texture = texture;
		context.bind();
		shader.setColor("textColor", color);
		startDraw();
	}
};

struct Character {
	Character() = default;
	mk::Texture        texture{};
	mk::math::Vector2u size{};     // size of glyph
	mk::math::Vector2u bearing{};  // offset from baseline to left/top of glyph
	u32                advance{};  // offset to advance next glyph
};

void renderText(
	const mk::RenderTarget2D&            target,
	std::string                          text,
	float                                x,
	float                                y,
	float                                scale,
	mk::Color                            color,
	std::unordered_map<char, Character>& chars
) {
	static FontArray vertex_array(false);
	vertex_array.setSize(6);
	vertex_array.color = color;

	// write verts
	for (char c: text) {
		const Character& ch   = chars.at(c);
		float            xpos = x + ch.bearing.x * scale;
		float            ypos = y + (ch.size.y - ch.bearing.y) * scale;
		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		std::array vertices = {
			FontVertex({ xpos, ypos }, { 0.0f, 0.0f }),
			FontVertex({ xpos, ypos + h }, { 0.0f, 1.0f }),
			FontVertex({ xpos + w, ypos + h }, { 1.0f, 1.0f }),

			FontVertex({ xpos, ypos }, { 0.0f, 0.0f }),
			FontVertex({ xpos + w, ypos + h }, { 1.0f, 1.0f }),
			FontVertex({ xpos + w, ypos }, { 1.0f, 0.0f }),
		};
		vertex_array.texture = &chars.at(c).texture;
		vertex_array.setVertexBuffer(vertices.data(), vertices.size());
		vertex_array.save();
		target.render2d(vertex_array);

		x += (ch.advance >> 6) * scale;
	}
}

int main() {
	mk::RenderWindow window(800, 600, "Text rendering");
	window.enableCamera2D(true);

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, mk::ResPath("fonts/Roboto/Roboto-Medium.ttf").strPath(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);


	std::unordered_map<char, Character> chars;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph: " << int(c) << std::endl;
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

		character.size    = mk::math::Vector2u(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		character.bearing = mk::math::Vector2u(face->glyph->bitmap_left, face->glyph->bitmap_top),
		character.advance = static_cast<u32>(face->glyph->advance.x);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!window.isExitRequested()) {
		window.clear(mk::Colors::DARK);
		renderText(window, "Test", 0.f, 0.f, 1.f, { mk::Colors::WHITE }, chars);
		window.display();
	}
}
