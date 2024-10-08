#include "Drawable.hpp"

#include "RenderTarget.hpp"

void mk::Drawable2D::draw(const RenderTarget& target) const { target.render(*this); }

void mk::Drawable3D::draw(const RenderTarget& target) const { target.render(*this); }
