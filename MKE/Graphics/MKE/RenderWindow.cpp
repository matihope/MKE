#include "RenderWindow.hpp"
#include "MKE/Event.hpp"

void mk::RenderWindow::enableCamera2D(bool enable) {
	if (!enable_camera_2d) {
		camera_transform(0, 3) = -1;
		camera_transform(1, 3) = 1;
		updateCamera();
	}

	enable_camera_2d = enable;
}

void mk::RenderWindow::addEvent(Event event) {
	Window::addEvent(event);
	if (enable_camera_2d
	    && (event.type == EventType::WindowResized
	        || event.type == EventType::WindowScaleFactorChanged))
		updateCamera();
}

void mk::RenderWindow::updateCamera() {
	auto [x_scale, y_scale] = (getScaleFactor() / getSize().type<float>()).bind();
	camera_transform(0, 0)  = x_scale;
	camera_transform(1, 1)  = -y_scale;
	setCamera(camera_transform);
}
