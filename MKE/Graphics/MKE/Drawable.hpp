#pragma once

namespace mk {
	class RenderTarget;
	class DrawContext;

    // Interface drawable, very nice
	class Drawable {
	public:
		virtual ~Drawable() = 0;

		virtual void draw(const RenderTarget& target, DrawContext context) = 0;
	};
}
