#include "RenderWindow.hpp"
#include "MKE/Event.hpp"

void mk::RenderWindow::render(const Drawable& drawable, DrawContext context) {
	glViewport(0, 0, window_size.x * window_scale_factor.x, window_size.y * window_scale_factor.y);
	defaultRender(drawable, context);
}

mk::math::Vector2u mk::RenderWindow::getSize() const { return Window::getSize(); }

mk::math::Vector2f mk::RenderWindow::getScalingFactor() const { return Window::getScalingFactor(); }
