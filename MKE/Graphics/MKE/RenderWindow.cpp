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

void mk::RenderWindow::render2d(const Drawable2D& drawable) const {
	DrawContext context;
	context.camera = camera_transform;
	render2d(drawable, context);
}

void mk::RenderWindow::render2d(const Drawable2D& drawable, DrawContext context) const {
	RenderTarget2D::render2d(drawable, context);
}

void mk::RenderWindow::render3d(const Drawable3D& drawable) const {
	DrawContext context;
	context.camera = camera_transform;
	render3d(drawable, context);
}

void mk::RenderWindow::render3d(const Drawable3D& drawable, DrawContext context) const {
	RenderTarget3D::render3d(drawable, context);
}
