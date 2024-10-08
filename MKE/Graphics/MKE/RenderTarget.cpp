#include "RenderTarget.hpp"
#include "Drawable.hpp"
#include "MKE/DrawContext.hpp"

void mk::RenderTarget::render(const Drawable& drawable) const { drawable.draw(*this); }

void mk::RenderTarget2D::render2d(const Drawable2D& drawable, DrawContext2D context) const {
	drawable.draw2d(*this, context);
}

void mk::RenderTarget3D::render3d(const Drawable3D& drawable, DrawContext3D context) const {
	drawable.draw3d(*this, context);
}
