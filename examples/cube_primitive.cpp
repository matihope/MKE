#include "MKE/DrawContext.hpp"
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
		= mk::math::perspective(45.f, float(width) / height, 0.01f, 10.f);

	std::array vertices = {
		mk::Vertex3D({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }),
		mk::Vertex3D({ 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }),
		mk::Vertex3D({ 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }),
		mk::Vertex3D({ 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }),
	};
	std::array indices = {
		0u, 1u, 2u, 0u, 2u, 3u,  // FRONT
		0u, 1u, 4u, 1u, 4u, 5u,  // FRONT
		1u, 2u, 5u, 2u, 5u, 6u,  //
		2u, 3u, 6u, 3u, 6u, 7u,  //
		3u, 0u, 7u, 0u, 7u, 4u,  //
		4u, 5u, 6u, 4u, 6u, 7u,  // BEHIND
	};

	mk::VertexArray3D cube(true);
	cube.setSize(8);
	cube.setVertexBuffer(vertices.data(), vertices.size());
	cube.setIndexBuffer(indices.data(), indices.size());
	cube.save();

	mk::Texture texture;
	texture.loadFromFile("arrow.png");
	texture.setSmooth(false);

	mk::DrawContext3D context;
	context.camera  = camera_projection;
	context.texture = &texture;

	class Transform: public mk::Transformable {
	public:
		Transform() = default;
	};

	Transform position;
	position.move(mk::math::Vector3f(-.5f));

	float time = 0.;
	while (!window.isExitRequested()) {
		const float radius = 5.0f;

		time += 1 / 60.f;

		float camX = sin(time) * radius;
		float camZ = cos(time) * radius;

		context.transform = position.getTransform();

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
