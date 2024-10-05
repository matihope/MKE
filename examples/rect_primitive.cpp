#include <MKE/Window.hpp>
#include "MKE/Event.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"

int main() {
	mk::RenderWindow window(800, 600, "RectPrimitive");

	auto [xscale, yscale] = window.getScaleFactor().bind();
	std::cout << "Size: " << window.getSize() << '\n';

	mk::math::Matrix4f camera;
	camera(0, 0) = xscale / window.getSize().x;
	camera(1, 1) = -yscale / window.getSize().y;
	camera(0, 3) = -1;
	camera(1, 3) = 1;
	window.setCamera(camera);

	mk::RectPrimitive rect(mk::math::Vector2f(400, 100));

	mk::Texture texture;
	texture.loadFromFile(mk::ResPath::example("arrow.png"));
	texture.setSmooth(false);

	bool run = true;

	while (run) {
		mk::Event event;
		while (window.pollEvent(event))
			if (event.type == mk::EventType::WindowClose) run = false;
		window.clear(mk::Colors::DARK);
		window.render(rect);
		window.display();
	}
}
