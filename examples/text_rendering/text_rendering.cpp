#include "MKE/Color.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Font.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"
#include "MKE/RenderTarget.hpp"
#include "MKE/RenderTexture.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/ResPath.hpp"
#include "MKE/Shader.hpp"
#include "MKE/Texture.hpp"
#include "MKE/VertexArray.hpp"
#include "glad/glad.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>

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
		static mk::Shader  shader(mk::ResPath("vert.vs"), mk::ResPath("frag.fs"));
		mk::math::Matrix4f transform{ 1 };
		transform(1, 1) = -1;
		transform(1, 3) = 50.f;

		context.shader  = &shader;
		context.texture = texture;
		context.transform *= transform;
		context.bind();
		shader.setColor("textColor", color);
		startDraw();
	}
};

void renderText(
	const mk::RenderTarget2D&                         target,
	std::string                                       text,
	float                                             x,
	float                                             y,
	float                                             scale,
	mk::Color                                         color,
	std::unordered_map<char8_t, mk::Font::Character>& chars
) {
	static FontArray vertex_array(false);
	vertex_array.setSize(6);
	vertex_array.color = color;

	// write verts
	for (char c: text) {
		const auto& ch   = chars.at(c);
		float       xpos = x + ch.bearing.x * scale;
		float       ypos = y + (ch.size.y - ch.bearing.y) * scale;
		float       w    = ch.size.x * scale;
		float       h    = ch.size.y * scale;

		std::array vertices = {
			FontVertex({ xpos, ypos }, { 0.0f, 1.0f }),
			FontVertex({ xpos, ypos + h }, { 0.0f, 0.0f }),
			FontVertex({ xpos + w, ypos + h }, { 1.0f, 0.0f }),

			FontVertex({ xpos, ypos }, { 0.0f, 1.0f }),
			FontVertex({ xpos + w, ypos + h }, { 1.0f, 0.0f }),
			FontVertex({ xpos + w, ypos }, { 1.0f, 1.0f }),
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

	mk::Font font;
	font.load("fonts/Roboto/Roboto-Medium.ttf");
	font.setScaling(window.getScaleFactor().x);
	font.setSize(64);

	mk::RenderTexture2D render_texture;
	render_texture.create(window.getSize());
	render_texture.setScalingFactor(window.getScaleFactor());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mk::RectPrimitive rect(800, 600);

	glDisable(GL_DEPTH_TEST);

	bool rendert = true;
	while (!window.isExitRequested()) {
		window.clear(mk::Colors::DARK);
		render_texture.clear(mk::Colors::TRANSPARENT);

		renderText(
			window,
			rendert ? "RenderT" : "Window",
			0.f,
			0.f,
			1.f / window.getScaleFactor().x,
			mk::Colors::WHITE,
			font.chars
		);

		mk::DrawContext context;
		context.camera  = window.getCamera();
		context.texture = &render_texture.getTexture();
		window.render2dContext(rect, context);

		window.display();
	}
}
