#pragma once

namespace mk {
	class RenderTarget;
	struct DrawContext;

	class Drawable {
	public:
		virtual void draw(const RenderTarget& target, DrawContext context) const = 0;
	};
}
