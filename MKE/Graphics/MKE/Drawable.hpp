#pragma once

namespace mk {
	class RenderTarget;
	struct DrawContext;

	// Interface drawable, very nice
	class Drawable {
	public:
		virtual void draw(const RenderTarget& target) const = 0;
	};

	class Drawable2D: public Drawable {
	public:
		void         draw(const RenderTarget& target) const override;
		virtual void draw2d(const RenderTarget& target, DrawContext context) const = 0;
	};

	class Drawable3D: public Drawable {
	public:
		void         draw(const RenderTarget& target) const override;
		virtual void draw3d(const RenderTarget& target, DrawContext context) const = 0;
	};

}
