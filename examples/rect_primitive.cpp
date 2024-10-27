#include "MKE/RenderWindow.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"

int main() {
	mk::RenderWindow window(800, 600, "RectPrimitive");

	mk::RectPrimitive rect(mk::math::Vector2f(400, 300));

	while (!window.isExitRequested()) {
		float diff_x
			= window.isKeyPressed(mk::input::KEY::D) - window.isKeyPressed(mk::input::KEY::A);
		float diff_y
			= window.isKeyPressed(mk::input::KEY::S) - window.isKeyPressed(mk::input::KEY::W);

		rect.move(mk::math::Vector2f{ diff_x, diff_y } * 5.f);

		window.clear(mk::Colors::DARK);
		window.render(rect, window.getCurrentView2D()->getTransform());
		window.display();
	}
}
