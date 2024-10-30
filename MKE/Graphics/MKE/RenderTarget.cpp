#include "RenderTarget.hpp"
#include "Drawable.hpp"
#include "MKE/DrawContext.hpp"

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

mk::math::Vector2f mk::RenderTarget::mapPixelToCoords2D(const math::Vector2i& point) const {
	
}
