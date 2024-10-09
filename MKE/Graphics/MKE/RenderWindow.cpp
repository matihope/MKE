#include "RenderWindow.hpp"
#include "MKE/Event.hpp"

void mk::RenderWindow::enableCamera2D(bool enable) {
	if (!enable_camera_2d) {
		camera_transform(0, 3) = -1;
		camera_transform(1, 3) = 1;
		updateCamera2D();
	}

	enable_camera_2d = enable;
}

void mk::RenderWindow::addEvent(Event event) {
	Window::addEvent(event);
	if (enable_camera_2d
	    && (event.type == EventType::WindowResized
	        || event.type == EventType::WindowScaleFactorChanged))
		updateCamera2D();
}

void mk::RenderWindow::updateCamera2D() {
	auto [width, height]   = getSize().bind();
	camera_transform(0, 0) = 2.f / width;
	camera_transform(1, 1) = -2.f / height;
	setCamera(camera_transform);
}
