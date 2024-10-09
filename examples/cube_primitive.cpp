#include "MKE/Color.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/MatrixUtils.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/Texture.hpp"
#include "MKE/Transformable.hpp"
#include "MKE/VertexArray.hpp"

int main() {
	mk::RenderWindow window(800, 600, "Cube");

	auto [width, height] = window.getSize().bind();
	const mk::math::Matrix4f camera_projection
		= mk::math::perspective(45.f, float(width) / height, 1.f, 100.f);

	std::array vertices = {
		mk::Vertex3D({ -0.5f, -0.5f, -0.5f }, { 1.f, 0.f, 0.f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ 0.5f, -0.5f, -0.5f }, { 0.f, 1.f, 0.f }, { 1.0f, 0.0f }),
		mk::Vertex3D({ 0.5f, 0.5f, -0.5f }, { 0.f, 0.f, 1.f }, { 1.0f, 1.0f }),
		mk::Vertex3D({ 0.5f, 0.5f, -0.5f }, { 1.f, 0.f, 0.f }, { 1.0f, 1.0f }),
		mk::Vertex3D({ -0.5f, 0.5f, -0.5f }, { 0.f, 1.f, 0.f }, { 0.0f, 1.0f }),
		mk::Vertex3D({ -0.5f, -0.5f, -0.5f }, { 0.f, 0.f, 1.f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ -0.5f, -0.5f, 0.5f }, { 1.f, 0.f, 0.f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ 0.5f, -0.5f, 0.5f }, { 0.f, 1.f, 0.f }, { 1.0f, 0.0f }),
		mk::Vertex3D({ 0.5f, 0.5f, 0.5f }, { 0.f, 1.f, 1.f }, { 1.0f, 1.0f }),
		mk::Vertex3D({ 0.5f, 0.5f, 0.5f }, { 1.f, 0.f, 0.f }, { 1.0f, 1.0f }),
		mk::Vertex3D({ -0.5f, 0.5f, 0.5f }, { 0.f, 1.f, 0.f }, { 0.0f, 1.0f }),
		mk::Vertex3D({ -0.5f, -0.5f, 0.5f }, { 0.f, 0.f, 1.f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ -0.5f, 0.5f, 0.5f }, { 1.f, 0.f, 0.f }, { 1.0f, 0.0f }),
		mk::Vertex3D({ -0.5f, 0.5f, -0.5f }, { 0.f, 1.f, 0.f }, { 1.0f, 1.0f }),
		mk::Vertex3D({ -0.5f, -0.5f, -0.5f }, { 0.f, 0.f, 1.f }, { 0.0f, 1.0f }),
		mk::Vertex3D({ -0.5f, -0.5f, -0.5f }, { 1.f, 0.f, 0.f }, { 0.0f, 1.0f }),
		mk::Vertex3D({ -0.5f, -0.5f, 0.5f }, { 0.f, 1.f, 0.f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ -0.5f, 0.5f, 0.5f }, { 0.f, 0.f, 1.f }, { 1.0f, 0.0f }),
		mk::Vertex3D({ 0.5f, 0.5f, 0.5f }, { 1.f, 0.f, 0.f }, { 1.0f, 0.0f }),
		mk::Vertex3D({ 0.5f, 0.5f, -0.5f }, { 0.f, 1.f, 0.f }, { 1.0f, 1.0f }),
		mk::Vertex3D({ 0.5f, -0.5f, -0.5f }, { 1.f, 0.f, 1.f }, { 0.0f, 1.0f }),
		mk::Vertex3D({ 0.5f, -0.5f, -0.5f }, { 1.f, 0.f, 0.f }, { 0.0f, 1.0f }),
		mk::Vertex3D({ 0.5f, -0.5f, 0.5f }, { 0.f, 1.f, 0.f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ 0.5f, 0.5f, 0.5f }, { 0.f, 0.f, 1.f }, { 1.0f, 0.0f }),
		mk::Vertex3D({ -0.5f, -0.5f, -0.5f }, { 1.f, 0.f, 0.f }, { 0.0f, 1.0f }),
		mk::Vertex3D({ 0.5f, -0.5f, -0.5f }, { 0.f, 1.f, 0.f }, { 1.0f, 1.0f }),
		mk::Vertex3D({ 0.5f, -0.5f, 0.5f }, { 0.f, 0.f, 1.f }, { 1.0f, 0.0f }),
		mk::Vertex3D({ 0.5f, -0.5f, 0.5f }, { 1.f, 0.f, 0.f }, { 1.0f, 0.0f }),
		mk::Vertex3D({ -0.5f, -0.5f, 0.5f }, { 0.f, 1.f, 0.f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ -0.5f, -0.5f, -0.5f }, { 0.f, 0.f, 1.f }, { 0.0f, 1.0f }),
		mk::Vertex3D({ -0.5f, 0.5f, -0.5f }, { 1.f, 0.f, 0.f }, { 0.0f, 1.0f }),
		mk::Vertex3D({ 0.5f, 0.5f, -0.5f }, { 0.f, 1.f, 0.f }, { 1.0f, 1.0f }),
		mk::Vertex3D({ 0.5f, 0.5f, 0.5f }, { 0.f, 0.f, 1.f }, { 1.0f, 0.0f }),
		mk::Vertex3D({ 0.5f, 0.5f, 0.5f }, { 1.f, 0.f, 0.f }, { 1.0f, 0.0f }),
		mk::Vertex3D({ -0.5f, 0.5f, 0.5f }, { 0.f, 1.f, 0.f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ -0.5f, 0.5f, -0.5f }, { 0.f, 0.f, 1.f }, { 0.0f, 1.0 }),
	};

	mk::VertexArray3D cube(false);
	cube.setSize(36);
	cube.setVertexBuffer(vertices.data(), vertices.size());
	cube.save();

	mk::Texture texture;
	texture.loadFromFile("arrow.png");
	texture.setSmooth(false);

	mk::DrawContext3D context;
	context.camera  = camera_projection;
	context.texture = &texture;

	float time = 0.;
	while (!window.isExitRequested()) {
		time += 1.f / 60.f;
		float radius = 5.f;

		float camX = sin(time) * radius;
		float camZ = cos(time) * radius;

		context.camera = camera_projection
		               * mk::math::lookAt(
							 mk::math::Vector3f(camX, 2.f, camZ),
							 mk::math::Vector3f(0.0f),
							 mk::math::Vector3f(0.f, 1.f, 0.f)
					   );

		window.clear(mk::Colors::DARK);
		window.render3d(cube, context);
		window.display();
	}
}
