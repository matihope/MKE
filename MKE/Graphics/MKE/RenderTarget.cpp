#include "RenderTarget.hpp"
#include "Drawable.hpp"
#include "MKE/DrawContext.hpp"
#include <stdexcept>

void mk::RenderTarget::defaultRender(const Drawable& drawable, DrawContext context) {
	drawable.draw(*this, context);
}

const mk::View2D& mk::RenderTarget::getCurrentView2D() const {
	if (custom_view2d) return *custom_view2d;
	return getDefaultView2D();
}

const mk::View3D& mk::RenderTarget::getCurrentView3D() const {
	if (custom_view3d) return *custom_view3d;
	return getDefaultView3D();
}

mk::math::Vector2f mk::RenderTarget::mapPixelToCoords2D(const math::Vector2i&) const {
	throw std::runtime_error("mapPixelToCoords2D is not yet implemented");
}

void mk::RenderTarget::setView2D(const View2D& view2d) { custom_view2d = view2d; }

void mk::RenderTarget::setView3D(const View3D& view3d) { custom_view3d = view3d; }

const mk::View2D& mk::RenderTarget::getDefaultView2D() const {
	static View2D view({ 0, 0 }, { 1, 1 });
	view.setSize(getSize().type<float>());
	view.setCenter(getSize().type<float>() / 2.f);
	return view;
}

const mk::View3D& mk::RenderTarget::getDefaultView3D() const {
	static View3D view(math::Vector3f{ 0.f }, math::Vector3f{ -1.f, 0.f, 0.f });
	return view;
}
