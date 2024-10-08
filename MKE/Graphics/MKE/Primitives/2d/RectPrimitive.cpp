#include "RectPrimitive.hpp"
#include "MKE/Math/Vector.hpp"

mk::RectPrimitive::RectPrimitive(math::Vector2f size): size(size) {
	vertices.setSize(4);

	auto&& v1     = vertices(0);
	v1.position   = { 0.0, 0.0 };
	v1.tex_coords = { 0.0, 0.0 };
	v1.color      = Colors::WHITE;
	auto&& v2     = vertices(1);
	v2.position   = { size.x, 0.0 };
	v2.tex_coords = { 1.0, 0.0 };
	v2.color      = Colors::WHITE;
	auto&& v3     = vertices(2);
	v3.position   = { size.x, size.y };
	v3.tex_coords = { 1.0, 1.0 };
	v3.color      = Colors::WHITE;
	auto&& v4     = vertices(3);
	v4.position   = { 0.0, size.y };
	v4.tex_coords = { 0.0, 1.0 };
	v4.color      = Colors::WHITE;

	std::array<u32, 6> indices = { 0, 2, 3, 0, 1, 2 };
	vertices.setIndexBuffer(indices.data(), indices.size());

	vertices.save();
}

mk::RectPrimitive::RectPrimitive(float width, float height):
	  mk::RectPrimitive(math::Vector2f{ width, height }) {}

void mk::RectPrimitive::draw2d(const RenderTarget& target, DrawContext2D context) const {
	context.transform *= getTransform();
	vertices.draw2d(target, context);
}
