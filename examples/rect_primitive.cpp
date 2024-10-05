#include <MKE/Window.hpp>
#include "MKE/Event.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"

int main() {
	mk::RenderWindow window(800, 600, "RectPrimitive");

	auto scale = window.getScaleFactor() / window.getSize().type<float>();

	mk::math::Matrix4f camera;
	camera(0, 0) = scale.x;
	camera(1, 1) = -scale.y;
	camera(0, 3) = -1;
	camera(1, 3) = 1;
	window.setCamera(camera);

	mk::RectPrimitive rect(mk::math::Vector2f(100, 100));

	mk::Texture texture;
	texture.loadFromFile("arrow.png");
	texture.setSmooth(false);

	bool run = true;

	while (run) {
		mk::Event event;
		while (window.pollEvent(event)) {
			if (event.type == mk::EventType::WindowClose) run = false;
			if (event.type == mk::EventType::WindowResized
			    || event.type == mk::EventType::WindowScaleFactorChanged) {
				std::cout << "WUT: " << window.getScaleFactor() << ' ' << window.getSize() << '\n';
				auto scale = window.getScaleFactor() / window.getSize().type<float>();

				mk::math::Matrix4f camera;
				camera(0, 0) = scale.x;
				camera(1, 1) = -scale.y;
				camera(0, 3) = -1;
				camera(1, 3) = 1;
				window.setCamera(camera);
			}
		}

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
