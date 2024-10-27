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

void mk::RenderWindow::render(const Drawable& drawable) {
	DrawContext context;
	context.camera = camera_transform;
	renderContext(drawable, context);
}

mk::math::Matrix4f mk::RenderWindow::getCamera() const { return camera_transform; }

void mk::RenderWindow::renderContext(const Drawable& drawable, DrawContext context) {
	glViewport(0, 0, window_size.x * window_scale_factor.x, window_size.y * window_scale_factor.y);
	defaultRender(drawable, context);
}

mk::math::Vector2u mk::RenderWindow::getSize() const { return Window::getSize(); }

mk::math::Vector2f mk::RenderWindow::getScalingFactor() const { return Window::getScalingFactor(); }
