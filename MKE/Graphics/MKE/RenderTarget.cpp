#include "RenderTarget.hpp"
#include "Drawable.hpp"
#include "MKE/DrawContext.hpp"

void mk::RenderTarget2D::defaultRender2D(const Drawable2D& drawable, DrawContext context) const {
	drawable.draw2d(*this, context);
}

void mk::RenderTarget3D::defaultRender3D(const Drawable3D& drawable, DrawContext context) const {
	drawable.draw3d(*this, context);
}
