#include <MKE/Window.hpp>

int main() {
	mk::Window window(800, 600, "Hello window!");

	while (!window.isExitRequested()) {
		window.clear(mk::Colors::DARK);
		window.display();
	}
}
