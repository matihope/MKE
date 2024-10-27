#include "RenderTarget.hpp"
#include "Drawable.hpp"
#include "MKE/DrawContext.hpp"

void mk::RenderTarget::defaultRender(const Drawable& drawable, DrawContext context) const {
	drawable.draw(*this, context);
}
