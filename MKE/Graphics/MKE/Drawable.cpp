#include "Drawable.hpp"

#include "RenderTarget.hpp"

void mk::Drawable2D::beginDraw(const RenderTarget& target) const { target.render(*this); }
