#include "MKE/DrawContext.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/Vertex.hpp"
#include "MKE/VertexArray.hpp"
#include "MKE/Transformable.hpp"

int main() {
	mk::RenderWindow window(800, 600, "Cube");

	mk::math::Matrix4f camera3d{ 1 };
	auto [width, height] = window.getSize().bind();

	float ratio = float(width) / float(height);
	float fovX  = 90.f;
	float front = 0.1f;   // NEAR
	float back  = 100.f;  // FAR

	float deg2rad = acos(-1.f) / 180.f;

	float tang  = tan(fovX / 2 * deg2rad);
	float right = front * tang;
	float top   = right / ratio;

	camera3d(0, 0) = front / right;
	camera3d(1, 1) = front / top;
	camera3d(2, 2) = (front + back) / (front - back);
	camera3d(3, 2) = -1.f;
	camera3d(0, 2) = 1.0;
	camera3d(1, 2) = (back + front) / (back - front);
	camera3d(2, 3) = 2 * back * front / (front - back);
	camera3d(3, 3) = 0.f;
	// camera3d(3, 3) = 0.9;

	std::array vertices = {
		mk::Vertex3D({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }),
		mk::Vertex3D({ 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }),
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

	mk::DrawContext3D context;
	context.camera = camera3d;

	class Transform: public mk::Transformable {
	public:
		Transform()  = default;
		~Transform() = default;
	};

	Transform cam_t;
	Transform pos_t;
	// pos_t.move(0.f, 0.f, -1.f);


	while (!window.isExitRequested()) {
		mk::math::Vector3f rotation{ 0.0002, 0.003, 0.004 };
		rotation *= 5.f;
		cam_t.rotate(rotation.x, rotation.y, rotation.z);
		context.camera = cam_t.getTransform() * camera3d;
		// context.camera    = camera3d;
		// context.transform = pos_t.getTransform();

		window.clear(mk::Colors::DARK);
		window.render3d(cube, context);
		window.display();
	}
}
