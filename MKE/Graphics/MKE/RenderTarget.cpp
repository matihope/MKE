#include "RenderTarget.hpp"
#include "Drawable.hpp"

void mk::RenderTarget::render(const Drawable& drawable) const { drawable.beginDraw(*this); }
