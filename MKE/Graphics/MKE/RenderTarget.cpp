#include "RenderTarget.hpp"
#include "Drawable.hpp"
#include "MKE/DrawContext.hpp"

void mk::RenderTarget::defaultRender(const Drawable& drawable, DrawContext context) {
	drawable.draw(*this, context);
}

const mk::View2D& mk::RenderTarget::getCurrentView2D() const { return current_view2d; }

const mk::View3D& mk::RenderTarget::getCurrentView3D() const { return current_view3d; }
