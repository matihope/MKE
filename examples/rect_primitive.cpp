#include <MKE/Window.hpp>
#include "MKE/Event.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"

int main() {
	mk::RenderWindow window(800, 600, "RectPrimitive");

    mk::RectPrimitive rect;

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
