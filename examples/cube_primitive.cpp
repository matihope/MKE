#include "MKE/DrawContext.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector.hpp"
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
	float front = 0.01f;  // NEAR
	float back  = 100.f;  // FAR

	float DEG2RAD = 2.f * M_PI / 360.f;

	float tangent = std::tan(fovX / 2 * DEG2RAD);
	float right   = front * tangent;
	float top     = right / ratio;

	camera3d(0, 0) = front / right;
	camera3d(1, 1) = front / top;
	camera3d(2, 2) = (front + back) / (front - back);
	camera3d(3, 2) = -1.f;
	camera3d(2, 3) = 2 * back * front / (front - back);
	camera3d(3, 3) = 0.f;

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
	pos_t.move(0.f, 0.f, 5.f);
	cam_t.rotate(0, M_PI, 0);


	while (!window.isExitRequested()) {
		mk::math::Vector3f rotation{ 0.0002, 0.003, 0.004 };
		rotation *= 5.f;

		float move_side
			= window.isKeyPressed(mk::input::KEY::D) - window.isKeyPressed(mk::input::KEY::A);
		float move_h
			= window.isKeyPressed(mk::input::KEY::S) - window.isKeyPressed(mk::input::KEY::W);

		mk::math::Vector4f vec(-move_side, 0, -move_h, 0);
		vec = (cam_t.getRotationTransform() * vec).normalizeOrZero() * 0.01;
		pos_t.move(vec.x, vec.y, vec.z);

		float look_side = window.isKeyPressed(mk::input::KEY::ARROW_RIGHT)
		                - window.isKeyPressed(mk::input::KEY::ARROW_LEFT);

		// float look_vert = window.isKeyPressed(mk::input::KEY::ARROW_UP)
		//                 - window.isKeyPressed(mk::input::KEY::ARROW_DOWN);
		cam_t.rotate(0.f, look_side * 0.01, 0.f);
		context.camera    = camera3d * cam_t.getTransform();
		context.transform = pos_t.getTransform();


		window.clear(mk::Colors::DARK);
		window.render3d(cube, context);
		window.display();
	}
}
