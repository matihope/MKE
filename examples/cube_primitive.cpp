#include "MKE/Math/Matrix.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/VertexArray.hpp"

int main() {
	mk::RenderWindow window(800, 600, "Cube");

	mk::math::Matrix4f camera3d{1};
	auto [x_scale, y_scale] = (window.getScaleFactor() / window.getSize().type<float>()).bind();
	camera3d(0, 0)  = x_scale;
	camera3d(1, 1)  = y_scale;
	window.setCamera(camera3d);

	mk::VertexArray2D

	while (!window.isExitRequested()) {

		float diff_x
			= window.isKeyPressed(mk::input::KEY::D) - window.isKeyPressed(mk::input::KEY::A);
		float diff_y
			= window.isKeyPressed(mk::input::KEY::S) - window.isKeyPressed(mk::input::KEY::W);

		rect.move(mk::math::Vector2f{ diff_x, diff_y } * 5.f);

		window.clear(mk::Colors::DARK);
		window.render(rect);
		window.display();
	}
}
