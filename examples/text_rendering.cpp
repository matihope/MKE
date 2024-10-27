#include "MKE/Input.hpp"
#include "MKE/Primitives/2d/Text.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/Font.hpp"

int main() {
	mk::RenderWindow window(800, 600, "Text rendering");

	mk::Font font;
	font.load("fonts/Roboto/Roboto-Medium.ttf");

	mk::Text2D text;
	text.setFont(&font);
	text.setCharacterSize(128);
	text.setCharacterScaling(window.getScalingFactor().x);

	while (!window.isExitRequested()) {
		window.clear(mk::Colors::DARK);

		if (window.isKeyPressed(mk::input::KEY::SPACE))
			text.setString("space");
		else
			text.setString("Test");

		window.render(text, window.getCurrentView2D()->getTransform());
		window.display();
	}
}
