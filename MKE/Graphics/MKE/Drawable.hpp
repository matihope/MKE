#pragma once

namespace mk {
	class RenderTarget2D;
	class RenderTarget3D;
	struct DrawContext;

	class Drawable2D {
	public:
		virtual void draw2d(const RenderTarget2D& target, DrawContext context) const = 0;
	};

	class Drawable3D {
	public:
		virtual void draw3d(const RenderTarget3D& target, DrawContext context) const = 0;
	};

}
