#include "MKE/Color.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Math/MatrixUtils.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Primitives/3d/CubePrimitive.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/Texture.hpp"

int main() {
	mk::RenderWindow window(800, 600, "Cube");

	auto [width, height] = window.getSize().bind();
	const mk::math::Matrix4f camera_projection
		= mk::math::perspective(45.f, float(width) / height, 0.1f, 100.f);

	mk::CubePrimitive cube;
	cube.move(mk::math::Vector3f{ -.5f });

	mk::Texture texture;
	texture.loadFromFile("arrow.png");
	texture.setSmooth(false);

	mk::DrawContext context;
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
		window.render(cube, context);
		window.display();
	}
}
