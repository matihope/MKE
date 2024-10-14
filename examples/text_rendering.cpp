#include "MKE/Primitives/2d/Text.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/Font.hpp"

int main() {
	mk::RenderWindow window(800, 600, "Text rendering");
	window.enableCamera2D(true);

	mk::Font font;
	font.load("fonts/Roboto/Roboto-Medium.ttf");

	mk::Text2D text;
	text.setFont(&font);
	text.setText("Test");
	text.setCharacterSize(64);
	text.setCharacterScaling(window.getScaleFactor().x);

	while (!window.isExitRequested()) {
		window.clear(mk::Colors::DARK);
		window.render2d(text);
		window.display();
	}
}
