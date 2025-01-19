#include "Text.hpp"
#include "MKE/Color.hpp"
#include "MKE/Math/Rect.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"
#include "MKE/VertexArray.hpp"
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

class FontArray: public mk::VertexArray<FontVertex> {
public:
	using VertexArray::VertexArray;
	~FontArray() = default;

	mk::Color          color{};
	const mk::Texture* texture = nullptr;
	float              y_delta = 0.0;

	void draw(mk::RenderTarget& target, mk::DrawContext context) const override {
		static mk::Shader  shader(vertex_shader, fragment_shader);
		mk::math::Matrix4f transform{ 1 };
		transform(1, 1) = -1;
		transform(1, 3) = y_delta;
		shader.setColor("textColor", color);

		context.shader  = &shader;
		context.texture = texture;
		context.transform *= transform;

		VertexArray::draw(target, context);
	}
};

void mk::Text2D::render() {
	text_bounds = math::RectF();
	render_texture.clear(Colors::TRANSPARENT);
	render_texture.setSmooth(font->isSmooth());

	if (isEmpty()) return;

	chars = &font->getChars(FontParams(char_size, char_scaling));

	static FontArray vertex_array(false);
	vertex_array.setSize(6);
	vertex_array.color = color;

	float x              = 0;
	vertex_array.y_delta = 0;

	std::queue<std::pair<char, math::RectF>> draw_queue;
	for (char c: text) {
		const auto& ch = chars->at(c);

		math::RectF char_bounds{};
		char_bounds.left = x + ch.bearing.x;
		char_bounds.top
			= -static_cast<float>(static_cast<i32>(ch.size.y) - static_cast<i32>(ch.bearing.y));
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
	render_texture.clear(Colors::TRANSPARENT);
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
		render_texture.render(vertex_array, render_texture.getCurrentView2D().getTransform());
	}
}

void mk::Text2D::draw(RenderTarget& target, DrawContext context) const {
	context.transform *= getTransform();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (!isEmpty()) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		context.texture = &render_texture.getTexture();
		target.render(render_object, context);
	}
}

void mk::Text2D::setFont(mk::Font* font) {
	if (this->font != font) {
		this->font = font;
		render();
	}
}

void mk::Text2D::setString(const std::string& text) {
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

const std::string& mk::Text2D::getString() const { return text; }

usize mk::Text2D::getCharacterSize() const { return char_size; }

mk::math::RectF mk::Text2D::getLocalBounds() const {
	return math::RectF(0, 0, text_bounds.width / char_scaling, text_bounds.height / char_scaling);
}

mk::math::RectF mk::Text2D::getGlobalBounds() const {
	auto transform    = getTransform();
	auto top_left     = math::Vector2f{ 0.f, 0.f };
	auto bottom_right = getLocalBounds().getSize();

	auto new_top_left     = transform ^ top_left;
	auto new_bottom_right = transform ^ bottom_right;

	auto min_x = std::min(new_top_left.x, new_bottom_right.x);
	auto max_x = std::max(new_top_left.x, new_bottom_right.x);
	auto min_y = std::min(new_top_left.y, new_bottom_right.y);
	auto max_y = std::max(new_top_left.y, new_bottom_right.y);

	return math::RectF(min_x, min_y, max_x - min_x, max_y - min_y);
}
