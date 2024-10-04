#pragma once

namespace mk {
	class RenderTarget;
	class DrawContext2D;

	// Interface drawable, very nice
	class Drawable {
	public:
		virtual void draw(const RenderTarget& target) const = 0;
	};

	class Drawable2D: public Drawable {
	public:
		void draw(const RenderTarget& target) const override;
		virtual void draw2d(const RenderTarget& target, DrawContext2D context) const = 0;
	};

}
