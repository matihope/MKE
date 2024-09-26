#include <MKE/Window.hpp>

int main() {
	mk::Window window(800, 600, "Hello window!");

	bool run = true;
	while (run) {
		mk::Event event;
		while (window.pollEvent(event))
			if (event.type == mk::EventType::WindowClose) run = false;

		window.clear(mk::Colors::DARK);
		window.display();
	}
}
