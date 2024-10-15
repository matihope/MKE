#include "Text.hpp"
#include "MKE/Color.hpp"
#include "MKE/Math/Rect.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"
#include "MKE/VertexArray.hpp"
#include <cmath>
#include <queue>

namespace {
	const char* vertex_shader
		= "#version 410 core\n"
		  "layout (location = 0) in vec2 pos;\n"
		  "layout (location = 1) in vec2 tex;\n"
		  "out vec2 tex_coords;\n"
		  "\n"
		  "uniform mat4 camera;\n"
		  "uniform mat4 transform;\n"
		  "\n"
		  "void main() {\n"
		  "    gl_Position = camera * transform * vec4(pos, 0.0, 1.0);\n"
		  "    tex_coords = tex;\n"
		  "}\n";
	const char* fragment_shader
		= "#version 410 core\n"
		  "\n"
		  "in vec2 tex_coords;\n"
		  "out vec4 color;\n"
		  "\n"
		  "uniform sampler2D text;\n"
		  "uniform vec4 textColor;\n"
		  "\n"
		  "void main() {\n"
		  "    color = vec4(textColor.xzy, textColor.a * texture(text, tex_coords).r);\n"
		  "}\n";
}

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

	mk::Color          color;
	const mk::Texture* texture;
	float              y_delta = 0.0;

	void draw2d(const mk::RenderTarget2D&, mk::DrawContext context) const override {
		static mk::Shader  shader(vertex_shader, fragment_shader);
		mk::math::Matrix4f transform{ 1 };
		transform(1, 1) = -1;
		transform(1, 3) = y_delta;

		context.shader  = &shader;
		context.texture = texture;
		context.transform *= transform;
		context.bind();
		shader.setColor("textColor", color);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
		startDraw();
	}
};

void mk::Text2D::render() {
	text_bounds = math::RectF();
	render_texture.clear(Colors::BLACK);

	if (isEmpty()) return;

	chars = &font->getChars(Font::FontParams(char_size, char_scaling));

	static FontArray vertex_array(false);
	vertex_array.setSize(6);
	vertex_array.color = color;

	float x              = 0;
	vertex_array.y_delta = 0;

	std::queue<std::pair<char, math::RectF>> draw_queue;
	for (char c: text) {
		const auto& ch = chars->at(c);

		math::RectF char_bounds{};
		char_bounds.left   = x + ch.bearing.x;
		char_bounds.top    = -float(ch.size.y - ch.bearing.y);
		char_bounds.width  = ch.size.x;
		char_bounds.height = ch.size.y;

		text_bounds.left     = std::min(text_bounds.left, char_bounds.left);
		text_bounds.top      = std::min(text_bounds.top, char_bounds.top);
		text_bounds.width    = std::max(text_bounds.width, char_bounds.right());
		text_bounds.height   = std::max(text_bounds.height, char_bounds.bottom());
		vertex_array.y_delta = std::max(vertex_array.y_delta, (float) ch.bearing.y);

		x += (ch.advance >> 6);

		draw_queue.emplace(c, char_bounds);
	}
	// Currently (width,height) are (right,bottom) of the rect
	text_bounds.width -= text_bounds.left;
	text_bounds.height -= text_bounds.top;

	render_texture.create(text_bounds.width, text_bounds.height);
	render_texture.setScalingFactor(char_scaling);
	render_object.setScale(text_bounds.width / char_scaling, text_bounds.height / char_scaling, 1);

	while (!draw_queue.empty()) {
		auto [c, bounds] = draw_queue.front();
		draw_queue.pop();

		std::array vertices = {
			FontVertex({ bounds.left, bounds.bottom() }, { 0.0f, 0.0f }),
			FontVertex({ bounds.left, bounds.top }, { 0.0f, 1.0f }),
			FontVertex({ bounds.right(), bounds.top }, { 1.0f, 1.0f }),

			FontVertex({ bounds.left, bounds.bottom() }, { 0.0f, 0.0f }),
			FontVertex({ bounds.right(), bounds.top }, { 1.0f, 1.0f }),
			FontVertex({ bounds.right(), bounds.bottom() }, { 1.0f, 0.0f }),
		};
		vertex_array.texture = &chars->at(c).texture;
		vertex_array.setVertexBuffer(vertices.data(), vertices.size());
		vertex_array.save();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		render_texture.render2d(vertex_array);
	}
}

void mk::Text2D::draw2d(const RenderTarget2D& target, DrawContext context) const {
	context.transform *= getTransform();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (!isEmpty()) {
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// target.render2dContext(render_object, context);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		context.texture = &render_texture.getTexture();
		target.render2dContext(render_object, context);
	}
}

void mk::Text2D::setFont(mk::Font* font) {
	if (this->font != font) {
		this->font = font;
		render();
	}
}

void mk::Text2D::setText(const std::string& text) {
	if (this->text != text) {
		this->text = text;
		render();
	}
}

void mk::Text2D::setColor(mk::Color color) {
	if (this->color != color) {
		this->color = color;
		render();
	}
}

bool mk::Text2D::isEmpty() const { return !font || !char_size || text.empty(); }

void mk::Text2D::setCharacterSize(usize size) {
	if (char_size != size) {
		char_size = size;
		render();
	}
}

void mk::Text2D::setCharacterScaling(float scale) {
	if (char_scaling != scale) {
		char_scaling = scale;
		render();
	}
}
