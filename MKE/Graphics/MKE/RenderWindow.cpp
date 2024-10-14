#include "RenderWindow.hpp"
#include "MKE/Event.hpp"
#include "MKE/RenderTarget.hpp"

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
	render2dContext(drawable, context);
}

void mk::RenderWindow::render3d(const Drawable3D& drawable) const {
	DrawContext context;
	context.camera = camera_transform;
	render3dContext(drawable, context);
}

mk::math::Matrix4f mk::RenderWindow::getCamera() const { return camera_transform; }

void mk::RenderWindow::render2dContext(const Drawable2D& drawable, DrawContext context) const {
	glViewport(0, 0, window_size.x * window_scale_factor.x, window_size.y * window_scale_factor.y);
	defaultRender2D(drawable, context);
}

void mk::RenderWindow::render3dContext(const Drawable3D& drawable, DrawContext context) const {
	glViewport(0, 0, window_size.x * window_scale_factor.x, window_size.y * window_scale_factor.y);
	defaultRender3D(drawable, context);
}
