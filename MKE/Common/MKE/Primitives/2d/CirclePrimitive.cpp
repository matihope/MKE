#include "MKE/Primitives/2d/CirclePrimitive.hpp"
#include "CirclePrimitive.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Vertex.hpp"

mk::CirclePrimitive::CirclePrimitive(float radius, usize point_count):
	  radius(radius),
	  point_count(point_count) {
	updateVertices();
}

void mk::CirclePrimitive::draw(RenderTarget& target, DrawContext context) const {
	context.transform *= getTransform();
	vertices.draw(target, context);
}

void mk::CirclePrimitive::updateVertices() {
	MK_ASSERT(point_count >= 3);
	std::vector<u32> indices(point_count, 0);
	for (usize i = 0; i < point_count; i++) {
		indices.push_back(0);
		indices.push_back(i);
		if (i + 1 == point_count)
			indices.push_back(1);
		else
			indices.push_back(i + 1);
	}
	vertices.setIndexBuffer(indices.data(), indices.size());

	vertices.setSize(point_count + 1);  // +1 for the middle point (0)

	Vertex2D& v0  = vertices(0);
	v0.position   = { 0.0, 0.0 };
	v0.tex_coords = { 0.5, 0.5 };
	v0.color      = color;

	const math::Vector2f top = { 0.f, 1.f };
	for (usize i = 0; i < point_count; i++) {
		auto      new_pos = math::rotateVector(top, float(i) / point_count * 2 * M_PI);
		Vertex2D& v       = vertices(i);
		v.position        = new_pos * radius;
		v.tex_coords      = { new_pos / 2.f + math::Vector2f{ 0.5 } };
		v.color           = color;
	}

	vertices.save();
}

void mk::CirclePrimitive::setRadius(float radius) {
	if (this->radius != radius) {
		this->radius = radius;
		updateVertices();
	}
}

void mk::CirclePrimitive::setPointCount(usize point_count) {
	if (this->point_count != point_count) {
		this->point_count = point_count;
		updateVertices();
	}
}

void mk::CirclePrimitive::setColor(Color color) {
	if (this->color != color) {
		this->color = color;
		updateVertices();
	}
}

float mk::CirclePrimitive::getRadius() const { return radius; }

usize mk::CirclePrimitive::getPointCount() const { return point_count; }
