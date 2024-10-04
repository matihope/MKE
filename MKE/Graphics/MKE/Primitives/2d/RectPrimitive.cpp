#include "RectPrimitive.hpp"

mk::RectPrimitive::RectPrimitive() {
	vertices.setSize(4);

	auto&& v1   = vertices(0);
	v1.position = { 0.0, 0.0 };
	v1.color    = Colors::WHITE;
	auto&& v2   = vertices(1);
	v2.position = { 1.0, 0.0 };
	v2.color    = Colors::WHITE;
	auto&& v3   = vertices(2);
	v3.position = { 1.0, 1.0 };
	v3.color    = Colors::WHITE;
	auto&& v4   = vertices(3);
	v4.position = { 0.0, 1.0 };
	v4.color    = Colors::WHITE;

	std::array<u32, 6> indices = { 0, 2, 3, 0, 1, 2};
	vertices.setIndexBuffer(indices.data(), indices.size());

	vertices.save();
}

void mk::RectPrimitive::draw2d(const RenderTarget& target, DrawContext2D context) const {
	context.transform = math::Matrix4f::multiply(context.transform, getTransform());
	target.render(vertices, context);
}
