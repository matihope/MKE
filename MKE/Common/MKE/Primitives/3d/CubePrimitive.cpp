#include "CubePrimitive.hpp"

mk::CubePrimitive::CubePrimitive(mk::Color color) {
	std::array vertices = {
		mk::Vertex3D({ 0.0f, 0.0f, 0.0f }, color, { 0.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 0.0f, 0.0f }, color, { 1.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 0.0f }, color, { 1.0f, 1.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 0.0f }, color, { 1.0f, 1.0f }),
		mk::Vertex3D({ 0.0f, 1.0f, 0.0f }, color, { 0.0f, 1.0f }),
		mk::Vertex3D({ 0.0f, 0.0f, 0.0f }, color, { 0.0f, 0.0f }),
		mk::Vertex3D({ 0.0f, 0.0f, 1.0f }, color, { 0.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 0.0f, 1.0f }, color, { 1.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 1.0f }, color, { 1.0f, 1.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 1.0f }, color, { 1.0f, 1.0f }),
		mk::Vertex3D({ 0.0f, 1.0f, 1.0f }, color, { 0.0f, 1.0f }),
		mk::Vertex3D({ 0.0f, 0.0f, 1.0f }, color, { 0.0f, 0.0f }),
		mk::Vertex3D({ 0.0f, 1.0f, 1.0f }, color, { 1.0f, 0.0f }),
		mk::Vertex3D({ 0.0f, 1.0f, 0.0f }, color, { 1.0f, 1.0f }),
		mk::Vertex3D({ 0.0f, 0.0f, 0.0f }, color, { 0.0f, 1.0f }),
		mk::Vertex3D({ 0.0f, 0.0f, 0.0f }, color, { 0.0f, 1.0f }),
		mk::Vertex3D({ 0.0f, 0.0f, 1.0f }, color, { 0.0f, 0.0f }),
		mk::Vertex3D({ 0.0f, 1.0f, 1.0f }, color, { 1.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 1.0f }, color, { 1.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 0.0f }, color, { 1.0f, 1.0f }),
		mk::Vertex3D({ 1.0f, 0.0f, 0.0f }, color, { 0.0f, 1.0f }),
		mk::Vertex3D({ 1.0f, 0.0f, 0.0f }, color, { 0.0f, 1.0f }),
		mk::Vertex3D({ 1.0f, 0.0f, 1.0f }, color, { 0.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 1.0f }, color, { 1.0f, 0.0f }),
		mk::Vertex3D({ 0.0f, 0.0f, 0.0f }, color, { 0.0f, 1.0f }),
		mk::Vertex3D({ 1.0f, 0.0f, 0.0f }, color, { 1.0f, 1.0f }),
		mk::Vertex3D({ 1.0f, 0.0f, 1.0f }, color, { 1.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 0.0f, 1.0f }, color, { 1.0f, 0.0f }),
		mk::Vertex3D({ 0.0f, 0.0f, 1.0f }, color, { 0.0f, 0.0f }),
		mk::Vertex3D({ 0.0f, 0.0f, 0.0f }, color, { 0.0f, 1.0f }),
		mk::Vertex3D({ 0.0f, 1.0f, 0.0f }, color, { 0.0f, 1.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 0.0f }, color, { 1.0f, 1.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 1.0f }, color, { 1.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 1.0f }, color, { 1.0f, 0.0f }),
		mk::Vertex3D({ 0.0f, 1.0f, 1.0f }, color, { 0.0f, 0.0f }),
		mk::Vertex3D({ 0.0f, 1.0f, 0.0f }, color, { 0.0f, 1.0 }),
	};

	vertex_array.setSize(vertices.size());
	vertex_array.setVertexBuffer(vertices.data(), vertices.size());
	vertex_array.save();
}

void mk::CubePrimitive::draw(const RenderTarget& target, DrawContext context) const {
	context.transform *= getTransform();
	vertex_array.draw(target, context);
}
