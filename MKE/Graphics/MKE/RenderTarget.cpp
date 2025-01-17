#include "RenderTarget.hpp"
#include "Drawable.hpp"
#include "MKE/DrawContext.hpp"
#include <stdexcept>

mk::View2D default_view_2d({ 0, 0 }, { 1, 1 });
mk::View3D default_view_3d(mk::math::Vector3f{ 0.f }, mk::math::Vector3f{ -1.f, 0.f, 0.f });

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
	default_view_2d.setSize(getSize().type<float>());
	default_view_2d.setCenter(getSize().type<float>() / 2.f);
	return default_view_2d;
}

const mk::View3D& mk::RenderTarget::getDefaultView3D() const {
	std::cerr << getSize() << '\n';
	default_view_3d.setAspect(getSize().x / getSize().y);
	return default_view_3d;
}
